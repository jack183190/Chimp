#include "Client.h"

namespace Chimp {
	Client::Client(const ConnectionInfo& serverInfo)
		: IClient(serverInfo)
	{
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
		}
		else {
			std::cerr << "Failed to connect to server. Failed enet_host_service." << std::endl;
			return;
		}

		m_IsValid = true;
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
		return m_IsValid;
	}

	void Client::PollEvents()
	{
		if (!IsValid()) {
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

		// Set id
		if (m_ConnectionId == INVALID_ID) {
			assert(type == NetworkPacketType::CLIENT_SET_ID);
			ToClientSetClientIdPacket* idPacket = reinterpret_cast<ToClientSetClientIdPacket*>(event.packet->data);
			m_ConnectionId = idPacket->NewClientId;
		}
		else {
			// Broadcast event
			NetworkPacket receiveEvent;
			//m_EventQueue.Push(std::make_tuple(receiveEvent, receiveEvent));
		}

		// Destroy packet
		enet_packet_destroy(event.packet);
	}
}