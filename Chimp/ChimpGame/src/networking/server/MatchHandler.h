#pragma once

#include "stdafx.h"
#include "MatchSet.h"

class MatchHandler
{
public:
	DISABLE_COPY(MatchHandler);
	MatchHandler(Chimp::Engine& engine, Chimp::IServer& server);
	~MatchHandler();

	void Update();

private:
	void HandleNewConnections(const Chimp::NetworkPacket* event);
	void StartMatch(int player1, int player2);

private:
	Chimp::Engine& m_Engine;
	Chimp::IServer& m_Server;
	std::queue<int> m_PlayersNotInMatch;
	MatchSet m_MatchSet;
	Chimp::EventListener m_NewConnectionListener;
};