#include "TowerManager.h"

TowerManager::TowerManager(
	Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition) :
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition),
	m_TowerTargetSystem(engine, ecs),
	m_TowerRotationSystem(engine, ecs),
	m_TowerProjectileSystem(engine, ecs)
{
}

void TowerManager::Update()
{
	m_TowerTargetSystem.Update();
	m_TowerRotationSystem.Update();
	m_TowerProjectileSystem.Update();
}

Chimp::EntityId TowerManager::PlaceTower(TowerType type, Chimp::Vector2f position)
{
	position += m_SimulationPosition;

	switch (type) {
	case TOWER_TYPE_DART_MONKEY:
		return Entities::CreateMonkeyTower(m_ECS, m_Engine.GetResourceManager().GetModel(std::string(GAME_SRC) + "/assets/models/monkey/MonkeyOBJ.obj"), position);
	default:
		assert(false);
		break;
	}
}