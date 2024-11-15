#include "TowerManager.h"

TowerManager::TowerManager(
	Chimp::Engine& engine, 
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition) : 
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition)
{
}

void TowerManager::Update()
{

}

Chimp::EntityId TowerManager::PlaceTower(TowerType type, Chimp::Vector2f position)
{
	position += m_SimulationPosition;

	switch (type) {
	case TowerType::MONKEY:
		return Entities::CreateMonkeyTower(m_ECS, m_Engine.GetResourceManager().GetModel(std::string(GAME_SRC) + "/assets/models/monkey/MonkeyOBJ.obj"), position);
	default:
		assert(false);
		break;
	}
}
