#pragma once

#include "stdafx.h"

class ClientCurrentMatchHandler
{
public:
	ClientCurrentMatchHandler(Chimp::Engine& engine, Chimp::IClient& client);
	~ClientCurrentMatchHandler();

	void Update();

private:
	void HandleMatchStart(const Chimp::NetworkPacket* event);

private:
	Chimp::Engine& m_Engine;
	Chimp::IClient& m_Client;
	Chimp::EventListener m_MatchStartListener;
};