#include "Server.h"

namespace Chimp {
	Server::Server(const ServerInfo& serverInfo)
		: IServer(serverInfo)
	{
		InitENet::Init();

		ENetAddress address = {};
		enet_address_set_host(&address, serverInfo.HostName.c_str());
		address.port = serverInfo.Port;


		m_Server = OptionalReference<ENetHost>(
			enet_host_create(
				serverInfo.IsHost ? &address : nullptr,
				serverInfo.IsHost ? serverInfo.MaxClients : 1,
				serverInfo.MaxChannels,
				serverInfo.MaxIncomingBandwidth,
				serverInfo.MaxOutgoingBandwidth
			));

		if (!m_Server) {
			std::cerr << "Failed to connect to/host ENet server." << std::endl;
			return;
		}

		// CONNECT TO SERVER
		const bool shouldConnect = !serverInfo.IsHost;
		if (shouldConnect) {
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

			// Send test packet
			ENetPacket* packet = enet_packet_create("Hello, server!", strlen("Hello, server!") + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(&m_Peer, 0, packet);
			enet_host_flush(&m_Server);
		}
		else {
			std::cout << "Hosting server." << std::endl;
		}

		m_IsValid = true;
	}

	Server::~Server()
	{
		if (m_Server)
		{
			enet_host_destroy(&m_Server);
		}
	}

	bool Server::IsValid() const
	{
		return m_IsValid;
	}

	void Server::PollEvents()
	{
		if (!IsValid()) {
			return;
		}

		ENetEvent event;
		while (enet_host_service(&m_Server, &event, 0) > 0)
		{
			std::cout << "call\n";
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				ConnectionEvent connectionEvent;
				m_EventQueue.Push(std::make_tuple(NetworkEventType::CONNECTED, connectionEvent));
				std::cout << "client connected, port: " << event.peer->address.port << std::endl;
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				NetworkEvent receiveEvent;
				m_EventQueue.Push(std::make_tuple(NetworkEventType::MESSAGE, receiveEvent));
				std::cout << "received packet with data: ";
				for (size_t i = 0; i < event.packet->dataLength; ++i) {
					std::cout << event.packet->data[i];
				}
				std::cout << std::endl;
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				ConnectionEvent disconnectionEvent;
				m_EventQueue.Push(std::make_tuple(NetworkEventType::DISCONNECTED, disconnectionEvent));
				std::cout << "client disconnected, port: " << event.peer->address.port << std::endl;
				break;
			default:
				break;
			}
		}
	}
}