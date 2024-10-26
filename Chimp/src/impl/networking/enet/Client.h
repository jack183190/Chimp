#pragma once

#include "api/networking/IClient.h"
#include "api/utils/OptionalReference.h"
#include "InitENet.h"

namespace Chimp {
	class Client : public IClient {
	public:
		Client(const ConnectionInfo& serverInfo);
		~Client();
	public:
		bool IsValid() const override;

	protected:
		void PollEvents() override;

	private:
		void HandleReceiveEvent(const ENetEvent& event);

	private:
		bool m_IsValid = false;
		OptionalReference<ENetHost> m_Server;
		OptionalReference<ENetPeer> m_Peer;
	};
}