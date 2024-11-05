#pragma once

#include "stdafx.h"
#include "client/ClientCurrentMatchHandler.h"
#include "rendering/GameRenderer.h"

struct ClientHandlers {
	std::unique_ptr<ClientCurrentMatchHandler> CurrentMatchHandler; // not null
};

class GameClient
{
public:
	GameClient(Chimp::Engine& engine);

	void Connect(Chimp::ConnectionInfo connectionInfo);

	bool IsConnected();
	void Disconnect() { m_Client.reset(); }

	Chimp::IClient& GetClient();
	ClientHandlers& GetHandlers();

private:
	void Update();

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<Chimp::IClient> m_Client;
	std::unique_ptr<ClientHandlers> m_Handlers;
};