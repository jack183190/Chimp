#pragma once

#include "GameServer.h"

GameServer::GameServer(Chimp::Engine& engine)
	: m_Engine(engine)
{

}

void GameServer::Start(Chimp::ConnectionInfo connectionInfo)
{
	m_Server = m_Engine.HostServer(connectionInfo);
	if (!m_Server)
	{
		GetLogger().Error("Failed to start server");
	}
}

void GameServer::Shutdown()
{
	if (m_Server)
	{
		m_Server.reset();
	}
}

void GameServer::Update()
{
	if (m_Server)
	{
		m_Server->Update();
	}
}

Chimp::IServer& GameServer::GetServer()
{
	assert(m_Server);
	return *m_Server;
}
