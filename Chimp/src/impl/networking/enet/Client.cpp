#include "Client.h"
#include "api/networking/PacketTypeRegistry.h"

namespace Chimp {
	Client::Client(const ConnectionInfo& serverInfo)
		: IClient(serverInfo)
	{
		const auto connectionStartTimestamp = std::chrono::high_resolution_clock::now();

		InitENet::Init();

		ENetAddress address = {};
		enet_address_set_host(&address, serverInfo.HostName.c_str());
		address.port = serverInfo.Port;

		// Create server
		m_Server = OptionalReference<ENetHost>(
			enet_host_create(
				nullptr,
				1,
				serverInfo.MaxChannels,
				serverInfo.MaxIncomingBandwidth,
				serverInfo.MaxOutgoingBandwidth
			));

		if (!m_Server) {
			std::cerr << "Failed to create ENet server." << std::endl;
			return;
		}

		// Server connection
		m_Peer = OptionalReference<ENetPeer>(
			enet_host_connect(&m_Server, &address, serverInfo.MaxChannels, 0)
		);

		if (!m_Peer) {
			std::cerr << "Failed to connect to server. Failed enet_host_connect." << std::endl;
			return;
		}

		// Attempt the connection
		ENetEvent event;
		if (enet_host_service(&m_Server, &event, serverInfo.ConnectionTimeoutMs) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			std::cout << "Connected to server." << std::endl;
			m_Connected = true;
		}
		else {
			std::cerr << "Failed to connect to server. Failed enet_host_service." << std::endl;
			return;
		}

		// Wait until we have our id (or we timeout)
		const auto msRemainingToConnect = std::chrono::milliseconds(serverInfo.ConnectionTimeoutMs) - (std::chrono::high_resolution_clock::now() - connectionStartTimestamp);

		std::mutex m_ReceiveIdMutex;
		std::unique_lock<std::mutex> lock(m_ReceiveIdMutex);
		m_ReceiveIdCv.wait_for(lock, msRemainingToConnect, [this]() { return m_ConnectionId != INVALID_ID; });
	}

	Client::~Client()
	{
		if (m_Server)
		{
			enet_host_destroy(&m_Server);
		}
		if (m_Peer)
		{
			enet_peer_reset(&m_Peer);
		}
	}

	bool Client::IsValid() const
	{
		return m_Connected && m_ConnectionId != INVALID_ID;
	}

	void Client::SendPacketToServer(const NetworkPacket& packet, int channel)
	{
		assert(IsValid());

		const auto packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);

		ENetPacket* enetPacket = enet_packet_create(&packet, packetSize, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(&m_Peer, channel, enetPacket);
	}

	void Client::PollEvents()
	{
		if (!m_Connected) {
			return;
		}

		ENetEvent event;
		while (enet_host_service(&m_Server, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				HandleReceiveEvent(event);
				break;
			}
		}
	}

	void Client::HandleReceiveEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_RECEIVE);
		// Get packet type
		NetworkPacketType type = *reinterpret_cast<NetworkPacketType*>(event.packet->data);
		assert(type != Packets::INVALID);

		// Set id
		if (m_ConnectionId == INVALID_ID) {
			assert(type == Packets::CLIENT_SET_ID);
			ToClientSetClientIdPacket* idPacket = reinterpret_cast<ToClientSetClientIdPacket*>(event.packet->data);
			m_ConnectionId = idPacket->NewClientId;
			m_ReceiveIdCv.notify_all();
		}
		else {
			// Broadcast event
			std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));
			m_EventQueue.Push(std::make_tuple(type, std::move(packet)));
		}

		// Destroy packet
		enet_packet_destroy(event.packet);
	}
}