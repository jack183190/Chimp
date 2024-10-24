#pragma once

#include "api/networking/IServer.h"
#include "api/utils/OptionalReference.h"
#include "InitENet.h"

namespace Chimp {
	class Server : public IServer {
	public:
		Server(const ServerInfo& serverInfo);
		~Server();
	public:
		bool IsValid() const override;

	protected:
		void PollEvents() override;

	private:
		bool m_IsValid = false;
		OptionalReference<ENetHost> m_Server;
		OptionalReference<ENetPeer> m_Peer;
	};
}