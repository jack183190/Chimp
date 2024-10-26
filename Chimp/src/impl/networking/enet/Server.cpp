#include "Server.h"

namespace Chimp {
	Server::Server(const ConnectionInfo& serverInfo)
		: IServer(serverInfo)
	{
		InitENet::Init();

		ENetAddress address = {};
		enet_address_set_host(&address, serverInfo.HostName.c_str());
		address.port = serverInfo.Port;


		m_Server = OptionalReference<ENetHost>(
			enet_host_create(
				&address,
				serverInfo.MaxClients,
				serverInfo.MaxChannels,
				serverInfo.MaxIncomingBandwidth,
				serverInfo.MaxOutgoingBandwidth
			));

		if (!m_Server) {
			std::cerr << "Failed to host ENet server." << std::endl;
			return;
		}

		std::cout << "Hosting server." << std::endl;

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

		// first int in the packet is event type

		ENetEvent event;
		while (enet_host_service(&m_Server, &event, 0) > 0)
		{
			std::cout << "received server event\n";
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				HandleConnectionEvent(event);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				HandleReceiveEvent(event);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				HandleDisconnectionEvent(event);
				break;
			default:
				break;
			}
		}
	}

	void Server::HandleConnectionEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_CONNECT);

		// Give the client an id
		int thisClientId = m_NextClientId++;
		m_ClientIds[event.peer] = thisClientId;


		IdPacket idPacket;
		idPacket.Id = thisClientId;

		ENetPacket* packet = enet_packet_create(&idPacket, sizeof(IdPacket), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(event.peer, 0, packet);

		// Broadcast the event
		ConnectionEvent connectionEvent;
		m_EventQueue.Push(std::make_tuple(NetworkEventType::CONNECTED, connectionEvent));
		std::cout << "client connected, port: " << event.peer->address.port <<
			", assigned id: " << thisClientId << std::endl;
	}

	void Server::HandleDisconnectionEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_DISCONNECT);

		// Remove the client id
		m_ClientIds.erase(event.peer);

		// Broadcast the event
		ConnectionEvent disconnectionEvent;
		m_EventQueue.Push(std::make_tuple(NetworkEventType::DISCONNECTED, disconnectionEvent));
		std::cout << "client disconnected, port: " << event.peer->address.port << std::endl;
	}

	void Server::HandleReceiveEvent(const ENetEvent& event)
	{
		// TODO
	}
}