#pragma once

#include "GameServer.h"

GameServer::GameServer(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetUpdateSubscriber().SubscribeUpdate([this]() {
		Update();
		});
}

void GameServer::Start(Chimp::ConnectionInfo connectionInfo)
{
	m_Server = m_Engine.HostServer(connectionInfo);
	if (!m_Server)
	{
		GetLogger().Error("Failed to start server");
	}
	else
	{
		m_Resources = std::make_unique<ServerResources>();
		m_Resources->MatchHandler = std::make_unique<MatchHandler>(m_Engine, *m_Server);
	}
}

void GameServer::Shutdown()
{
	if (m_Server)
	{
		assert(m_Resources);
		m_Resources.reset();
		m_Server.reset();
	}
}

void GameServer::Update()
{
	if (m_Server)
	{
		assert(m_Resources);
		m_Server->Update();
		m_Resources->MatchHandler->Update();
	}
}

Chimp::IServer& GameServer::GetServer()
{
	assert(m_Server);
	return *m_Server;
}

ServerResources& GameServer::GetResources()
{
	assert(m_Resources);
	return *m_Resources;
}
