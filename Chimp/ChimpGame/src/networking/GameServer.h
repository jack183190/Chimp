#pragma once

#include "stdafx.h"
#include "server/MatchHandler.h"

struct ServerHandlers {
	std::unique_ptr<MatchHandler> MatchHandler; // not null
};

class GameServer
{
public:
	GameServer(Chimp::Engine& engine);

	void Start(Chimp::ConnectionInfo connectionInfo);

	bool IsRunning() const { return m_Server != nullptr; }
	void Shutdown();

	Chimp::IServer& GetServer();
	ServerHandlers& GetHandlers();

private:
	void Update();

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<Chimp::IServer> m_Server;
	std::unique_ptr<ServerHandlers> m_Handlers;
};