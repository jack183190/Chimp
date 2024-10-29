#pragma once

#include "Chimp.h"

class GameNetworking
{
public:
	GameNetworking() = default;

private:
	std::shared_ptr<Chimp::IServer> m_Server;
	std::shared_ptr<Chimp::IClient> m_Client;
};