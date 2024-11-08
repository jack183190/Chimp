#pragma once

#include "stdafx.h"
#include "rendering/GameRenderer.h"
#include "Entities.h"
#include "entities/health/HealthSystem.h"
#include "BloonManager.h"
#include "waves/WaveManagerBuilder.h"

class Simulation {
public:
	Simulation(Chimp::Engine& engine, 
		std::shared_ptr<GameRenderer> gameRenderer,
		Chimp::Vector2f position);

	void Update();
	void Render();
	void RenderUI();

	bool HasLost() const;
private:
	Chimp::Engine& m_Engine;
	Chimp::Vector2f m_Position;
	Chimp::ECS m_ECS;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	HealthSystem m_HealthSystem;
	BloonManager m_BloonManager;
	std::unique_ptr<Chimp::WaveManager> m_WaveManager;
};