#pragma once

#include "stdafx.h"

class GameClient
{
public:
	GameClient(Chimp::Engine& engine);

	void Connect(Chimp::ConnectionInfo connectionInfo);

	bool IsConnected();
	void Disconnect() { m_Client.reset(); }

	Chimp::IClient& GetClient();

private:
	void Update();

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<Chimp::IClient> m_Client;
};