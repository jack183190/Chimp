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
		m_EventQueue.PopAll([this](const std::tuple<NetworkPacketType, NetworkPacket>& event) {
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(std::get<0>(event), std::get<1>(event));
			});
	}
}