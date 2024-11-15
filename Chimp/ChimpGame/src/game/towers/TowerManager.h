#pragma once

#include "stdafx.h"
#include "game/Entities.h"

class TowerManager {
public:
	TowerManager(
		Chimp::Engine& engine, 
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition
	);

	void Update();

	Chimp::EntityId PlaceTower(TowerType type, Chimp::Vector2f position);

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
	Chimp::Vector2f m_SimulationPosition;
};