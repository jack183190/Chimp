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

		void SendPacketToServer(const NetworkPacket& packet, int channel = 0) override;
		void SendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0) override;

	protected:
		void PollEvents() override;

	private:
		void HandleReceiveEvent(const ENetEvent& event);

	private:
		bool m_Connected = false;
		OptionalReference<ENetHost> m_Server;
		OptionalReference<ENetPeer> m_Peer;
		std::condition_variable m_ReceiveIdCv;
		int m_CallbackIdCounter = 0;
		std::unordered_map<int, std::function<void(const NetworkPacket*)>> m_AwaitingResponseCallbacks;
		int m_HandlingCallbackId = 0;
		bool m_IsHandlingCallback = false;
	};
}