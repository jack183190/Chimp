#pragma once

#include "TowerManager.h"

class TowerEditor {
public:
	TowerEditor(TowerManager& towerManager,
		Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition);

	void Update();

private:
	void Place(TowerType type, Chimp::Vector2f position);

private:
	TowerManager& m_TowerManager;
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
	Chimp::Vector2f m_SimulationPosition;
};