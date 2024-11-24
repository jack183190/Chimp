#pragma once

#include "stdafx.h"
#include "BloonManager.h"
#include "game/Entities.h"

class BloonSpawner {
public:
	BloonSpawner(Chimp::Engine& engine, BloonManager& opponentBloonManager);

	void Update();
	void RenderUI();

private:
	void SendBloon(Bloons::BloonType bloonType, float delay = 0);

private:
	Chimp::Engine& m_Engine;
	BloonManager& m_OpponentBloonManager;
	float m_BloonSendsDisabledTime = 0;
};