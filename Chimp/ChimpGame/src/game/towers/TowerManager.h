#pragma once

#include "stdafx.h"
#include "game/Entities.h"
#include "systems/TowerAttackSystem.h"
#include "systems/TowerTargetSystem.h"
#include "systems/TowerProjectileSystem.h"

class TowerManager {
public:
	TowerManager(
		Chimp::Engine& engine, 
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition,
		Chimp::Vector2f simulationSize
	);

	void Update();

	Chimp::EntityId PlaceTower(TowerType type, Chimp::Vector2f position);
	Chimp::EntityId PlaceTowerWithNetworkId(TowerType type, Chimp::Vector2f position, NetworkId id);
	void RemoveTowerWithNetworkId(NetworkId id);

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
	Chimp::Vector2f m_SimulationPosition;
	TowerTargetSystem m_TowerTargetSystem;
	TowerAttackSystem m_TowerRotationSystem;
	TowerProjectileSystem m_TowerProjectileSystem;
};