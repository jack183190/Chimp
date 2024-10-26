#include "Server.h"
#include "networking/PacketTypeRegistry.h"

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

	void Server::SendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel)
	{
		assert(IsValid());

		ENetPeer* peer = m_ClientIdsReverse.at(clientId);
		assert(peer);

		const auto packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);

		ENetPacket* enetPacket = enet_packet_create(&packet, packetSize, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, channel, enetPacket);
	}

	void Server::SendPacketToAllClients(const NetworkPacket& packet, int channel)
	{
		for (auto& [peer, clientId] : m_ClientIds)
		{
			SendPacketToClient(clientId, packet, channel);
		}
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
		ToClientSetClientIdPacket idPacket;
		idPacket.PacketType = Packets::CLIENT_SET_ID;
		idPacket.NewClientId = m_NextClientId++;
		m_ClientIds[event.peer] = idPacket.NewClientId;
		m_ClientIdsReverse[idPacket.NewClientId] = event.peer;

		SendPacketToClient(idPacket.NewClientId, idPacket);
	}

	void Server::HandleDisconnectionEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_DISCONNECT);

		// Remove the client id
		int clientId = m_ClientIds[event.peer];
		m_ClientIdsReverse.erase(clientId);
		m_ClientIds.erase(event.peer);
	}

	void Server::HandleReceiveEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_RECEIVE);

		// Broadcast event
		std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));
		assert(packet->PacketType != Packets::INVALID);
		m_EventQueue.Push(std::make_tuple(packet->PacketType, std::move(packet)));

		// Destroy packet
		enet_packet_destroy(event.packet);
	}
}