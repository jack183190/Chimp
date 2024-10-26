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
		int m_NextClientId = 0;
	};
}