#pragma once

#include "api/networking/IServerConnection.h"

namespace Chimp {
	IServerConnection::IServerConnection(const ConnectionInfo& serverInfo) :
		m_ServerInfo(serverInfo),
		m_EventPollingThread([this]() {
		while (!m_IsBeingDestroyed) {
			PollEvents();
		}
			}) {

	}

	IServerConnection::~IServerConnection()
	{
		m_IsBeingDestroyed = true;
		m_EventPollingThread.join();
	}

	void IServerConnection::Update()
	{
		m_EventQueue.PopAll([this](const std::tuple<NetworkEventType, NetworkEvent>& event) {
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(std::get<0>(event), std::get<1>(event));
			});
	}
}