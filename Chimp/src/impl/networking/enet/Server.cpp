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

	void Server::SendPacketToClient(int clientId, const NetworkPacket& packet, int channel)
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

	void Server::SendPacketToAllClientsExcept(const NetworkPacket& packet, const std::vector<int> &excludedClients, int channel)
	{
		for (auto& [peer, clientId] : m_ClientIds)
		{
			if (std::find(excludedClients.begin(), excludedClients.end(), clientId) == excludedClients.end())
			{
				SendPacketToClient(clientId, packet, channel);
			}
		}

	}

	void Server::SendPacketToClientWithResponse(int clientId, const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel)
	{
		int callbackId = m_CallbackIdCounter++;
		auto& thisClientCallbacks = m_AwaitingResponseCallbacks[m_ClientIdsReverse.at(clientId)];
		thisClientCallbacks[callbackId] = callback;

		ToServerRequestResponsePacket requestPacket;
		requestPacket.PacketType = Packets::SERVER_REQUEST_RESPONSE;
		requestPacket.RequestId = callbackId;

		std::cout << "sending packet to client and requesting response\n";

		SendPacketToClient(clientId, requestPacket, channel);
		SendPacketToClient(clientId, packet, channel);
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
		m_AwaitingResponseCallbacks[event.peer] = std::unordered_map<int, std::function<void(const NetworkPacket*)>>();

		SendPacketToClient(idPacket.NewClientId, idPacket);

		// Broadcast connection
		ClientConnectedPacket connectPacket;
		connectPacket.PacketType = Packets::CLIENT_CONNECTED;
		connectPacket.ClientId = idPacket.NewClientId;
		SendPacketToAllClientsExcept(connectPacket, { idPacket.NewClientId });
	}

	void Server::HandleDisconnectionEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_DISCONNECT);

		// Remove the client id
		int clientId = m_ClientIds[event.peer];
		m_ClientIdsReverse.erase(clientId);
		m_ClientIds.erase(event.peer);
		m_RespondToPacketId.erase(event.peer);
		auto& thisClientCallbacks = m_AwaitingResponseCallbacks[event.peer];
		for (auto& [callbackId, callback] : thisClientCallbacks) {
			callback(nullptr); // Call it with nullptr to indicate the client disconnected
		}
		m_AwaitingResponseCallbacks.erase(event.peer);

		// Broadcast disconnection
		ClientDisconnectedPacket disconnectPacket;
		disconnectPacket.PacketType = Packets::CLIENT_DISCONNECTED;
		disconnectPacket.ClientId = clientId;
		SendPacketToAllClients(disconnectPacket);
	}

	void Server::HandleReceiveEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_RECEIVE);

		// Parse packet
		std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));
		assert(packet->PacketType != Packets::INVALID);

		// Forward or broadcast packet
		if (packet->PacketType == Packets::FORWARD)
		{
			ToServerForwardPacket* forwardPacket = reinterpret_cast<ToServerForwardPacket*>(packet.get());
			m_ForwardNextPacketToClientId = forwardPacket->ClientId;
		}
		else if (m_ForwardNextPacketToClientId != INVALID_ID)
		{
			SendPacketToClient(m_ForwardNextPacketToClientId, *packet);
			m_ForwardNextPacketToClientId = INVALID_ID;
		}
		// Handle client requesting a response
		else if (packet->PacketType == Packets::CLIENT_REQUEST_RESPONSE)
		{
			ToServerRequestResponsePacket* responsePacket = reinterpret_cast<ToServerRequestResponsePacket*>(packet.get());
			assert(responsePacket);

			// Mark next packet as one that needs to be responded to
			m_RespondToPacketId[event.peer] = responsePacket->RequestId;

			std::cout << "server will respond to next packet" << std::endl;
		}
		else if (m_RespondToPacketId.find(event.peer) != m_RespondToPacketId.end()) {
			// Get request id
			auto requestId = m_RespondToPacketId[event.peer];

			// Remove request id
			m_RespondToPacketId.erase(event.peer);

			// Get our response
			auto iter = m_PacketResponseHandlers.find(packet->PacketType);
			assert(iter != m_PacketResponseHandlers.end());
			PacketResponseFunc& responseFunc = iter->second;
			std::unique_ptr<NetworkPacket> responsePacket = responseFunc(packet.get());

			// Tell them our next packet is a response
			ToClientServerRespondingPacket response;
			response.PacketType = Packets::SERVER_RESPONDING_TO_CLIENT;
			response.RequestId = requestId;
			SendPacketToClient(m_ClientIds[event.peer], response);

			// Send the response
			SendPacketToClient(m_ClientIds[event.peer], *responsePacket);

			std::cout << "server responded to packet whos request id was " << requestId << std::endl;
		}
		// Handle client responding to us
		else if (packet->PacketType == Packets::CLIENT_RESPONDING_TO_SERVER) {
			// Make the next packet call the callback
			m_IsHandlingCallback = true;
			m_HandlingCallbackId = reinterpret_cast<ToServerClientRespondingPacket*>(packet.get())->RequestId;
		}
		else if (m_IsHandlingCallback) {
			// Handle the callback
			auto& thisClientCallbacks = m_AwaitingResponseCallbacks[event.peer];
			auto iter = thisClientCallbacks.find(m_HandlingCallbackId);
			m_IsHandlingCallback = false;
			if (iter != thisClientCallbacks.end()) {
				iter->second(packet.get());
				thisClientCallbacks.erase(iter);
			}
			else {
				assert(false);
			}
		}

		m_EventQueue.Push(std::make_tuple(packet->PacketType, std::move(packet)));

		// Destroy packet
		enet_packet_destroy(event.packet);
	}
}