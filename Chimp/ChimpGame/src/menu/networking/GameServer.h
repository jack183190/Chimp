#pragma once

#include "stdafx.h"

class GameServer
{
public:
	GameServer(Chimp::Engine& engine);

	void Start(Chimp::ConnectionInfo connectionInfo);

	bool IsRunning() const { return m_Server != nullptr; }
	void Shutdown();

	void Update();

	Chimp::IServer& GetServer();
private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<Chimp::IServer> m_Server;
};