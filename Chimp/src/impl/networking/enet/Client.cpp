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

	void Client::ImplSendPacketToServer(const NetworkPacket& packet, int channel)
	{
		// todo multithreading support here
		assert(IsValid());

		const auto packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);

		ENetPacket* enetPacket = enet_packet_create(&packet, packetSize, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(&m_Peer, channel, enetPacket);
	}

	void Client::ImplSendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel)
	{
		int callbackId = m_CallbackIdCounter++;
		m_AwaitingResponseCallbacks[callbackId] = callback;

		ToServerRequestResponsePacket requestPacket;
		requestPacket.PacketType = Packets::CLIENT_REQUEST_RESPONSE;
		requestPacket.RequestId = callbackId;

		ImplSendPacketToServer(requestPacket, channel);
		ImplSendPacketToServer(packet, channel);
	}

	void Client::AsyncUpdate()
	{
		if (!m_Connected) {
			return;
		}

		// Send queued packets
		if (m_SendQueuedPackets) {
			m_QueuedPacketsToSend.PopAll([this](const std::function<void()>& packet) {
				packet();
				});
			m_SendQueuedPackets = false;
		}

		// Poll events
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
		// CLIENT ASKING SERVER FOR RESPONSE
		else if (type == Packets::SERVER_RESPONDING_TO_CLIENT) {
			// Make the next packet call the callback
			m_IsHandlingCallback = true;
			m_HandlingCallbackId = reinterpret_cast<ToClientServerRespondingPacket*>(event.packet->data)->RequestId;
		}
		else if (m_IsHandlingCallback) {
			// Handle the callback
			auto iter = m_AwaitingResponseCallbacks.find(m_HandlingCallbackId);
			m_IsHandlingCallback = false;
			if (iter != m_AwaitingResponseCallbacks.end()) {
				std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));
				iter->second(packet.get());
				m_AwaitingResponseCallbacks.erase(iter);
			}
			else {
				assert(false);
			}
		}
		// SERVER ASKING CLIENT FOR RESPONSE
		else if (type == Packets::SERVER_REQUEST_RESPONSE) {
			std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));
			ToClientRequestResponsePacket* responsePacket = reinterpret_cast<ToClientRequestResponsePacket*>(packet.get());

			// Mark next packet as one that needs to be responded to
			m_RespondingToPacket = true;
			m_RespondToPacketId = responsePacket->RequestId;

			std::cout << "client will respond to next packet" << std::endl;
		}
		else if (m_RespondingToPacket) {
			std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));

			// Get our response
			auto iter = m_PacketResponseHandlers.find(type);
			assert(iter != m_PacketResponseHandlers.end());
			PacketResponseFunc& responseFunc = iter->second;
			std::unique_ptr<NetworkPacket> responsePacket = responseFunc(packet.get());

			// Tell them our next packet is a response
			ToServerClientRespondingPacket response;
			response.PacketType = Packets::CLIENT_RESPONDING_TO_SERVER;
			response.RequestId = m_RespondToPacketId;
			ImplSendPacketToServer(response);

			// Send the response
			ImplSendPacketToServer(*responsePacket);

			m_RespondingToPacket = false;
			std::cout << "client responded to packet whos request id was " << m_RespondToPacketId << std::endl;
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