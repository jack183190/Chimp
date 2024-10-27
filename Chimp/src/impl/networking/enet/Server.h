#pragma once

#include "api/networking/IServer.h"
#include "api/utils/OptionalReference.h"
#include "InitENet.h"

namespace Chimp {
	class Server : public IServer {
	public:
		Server(const ConnectionInfo& serverInfo);
		~Server();
	public:
		bool IsValid() const override;

		void SendPacketToClient(int clientId, const NetworkPacket& packet, int channel = 0) override;

		void SendPacketToAllClients(const NetworkPacket& packet, int channel = 0) override;

		void SendPacketToAllClientsExcept(const NetworkPacket& packet, const std::vector<int>& excludedClients, int channel = 0) override;

	protected:
		void PollEvents() override;

	private:
		void HandleConnectionEvent(const ENetEvent& event);
		void HandleDisconnectionEvent(const ENetEvent& event);
		void HandleReceiveEvent(const ENetEvent& event);

	private:
		bool m_IsValid = false;
		OptionalReference<ENetHost> m_Server;
		std::unordered_map<ENetPeer*, int> m_ClientIds;
		std::unordered_map<int, ENetPeer*> m_ClientIdsReverse;
		int m_NextClientId = 0;
		int m_ForwardNextPacketToClientId = INVALID_ID;
		std::unordered_map<ENetPeer*, int> m_RespondToPacketId; // client, request id
	};
}