#include "GameClient.h"

GameClient::GameClient(Chimp::Engine& engine) : m_Engine(engine)
{
}

void GameClient::Connect(Chimp::ConnectionInfo connectionInfo)
{
	m_Client = m_Engine.ConnectToServer(connectionInfo);
	if (!m_Client)
	{
		GetLogger().Error("Failed to connect to server");
	}
}

bool GameClient::IsConnected()
{
	if (m_Client && m_Client->WasDisconnected())
	{
		m_Client.reset();
	}
	return m_Client != nullptr;
}

void GameClient::Update()
{
	if (m_Client)
	{
		m_Client->Update();
	}
}

Chimp::IClient& GameClient::GetClient()
{
	bool connected = IsConnected();
	assert(connected);
	return *m_Client;
}
