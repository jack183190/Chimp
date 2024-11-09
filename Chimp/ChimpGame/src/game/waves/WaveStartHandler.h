#pragma once

#include "stdafx.h"
#include "networking/Networking.h"

class WaveStartHandler {
public:
	WaveStartHandler(Chimp::WaveManager& playerWaveManager,
		Chimp::WaveManager& opponentWaveManager);
	
	void Update();

private:
	Chimp::WaveManager& m_PlayerWaveManager;
	Chimp::WaveManager& m_OpponentWaveManager;
};