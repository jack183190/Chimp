#pragma once

#include "api/networking/IServer.h"

namespace Chimp {
	unsigned int IServer::ServerIdCounter = 0;

	IServer::IServer(const ServerInfo& serverInfo) :
		m_ServerInfo(serverInfo),
		m_ServerLocalId(ServerIdCounter++),
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
		m_EventQueue.PopAll([this](const std::tuple<NetworkEventType, NetworkEvent>& event) {
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(std::get<0>(event), std::get<1>(event));
			});
	}
}