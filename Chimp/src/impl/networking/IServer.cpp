#pragma once

#include "api/networking/IServer.h"

namespace Chimp {
	IServer::IServer(const ConnectionInfo& serverInfo) :
		m_ServerInfo(serverInfo),
		m_EventPollingThread([this]() {
		while (!m_IsBeingDestroyed) {
			PollEvents();
		}
			}) {

	}

	IServer::~IServer()
	{
		m_IsBeingDestroyed = true;
		m_EventPollingThread.join();
	}

	void IServer::Update()
	{
		m_EventQueue.PopAll([this](const std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>& event) {
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(std::get<0>(event), *std::get<1>(event));
			});
	}
}