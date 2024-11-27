#pragma once

#include "stdafx.h"
#include "networking/Networking.h"
#include "game/Simulation.h"
#include "rendering/GameRenderer.h"

class MatchWinLoseHandler {
public:
	MatchWinLoseHandler(Chimp::Engine& engine, 
		Simulation& playerSimulation,
		std::shared_ptr<GameRenderer> renderer);

	void Update();

private:
	Chimp::Engine& m_Engine;
	Simulation& m_PlayerSimulation;
	std::shared_ptr<GameRenderer> m_GameRenderer;
};