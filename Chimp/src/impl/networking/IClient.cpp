#pragma once

#include "api/networking/IClient.h"

namespace Chimp {
	IClient::IClient(const ConnectionInfo& serverInfo) :
		m_ServerInfo(serverInfo),
		m_EventPollingThread([this]() {
		while (!m_IsBeingDestroyed) {
			PollEvents();
		}
			}) {

	}

	IClient::~IClient()
	{
		m_IsBeingDestroyed = true;
		m_EventPollingThread.join();
	}

	void IClient::Update()
	{
		m_EventQueue.PopAll([this](const std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>& event) {
			auto eventType = std::get<0>(event);
			auto& eventPacket = *std::get<1>(event);
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(eventType, eventPacket);
			});
	}
}