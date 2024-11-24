#include "TowerManager.h"

TowerManager::TowerManager(
	Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition,
	Chimp::Vector2f simulationSize) :
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition),
	m_TowerTargetSystem(engine, ecs),
	m_TowerRotationSystem(engine, ecs),
	m_TowerProjectileSystem(engine, ecs, simulationPosition, simulationSize)
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

Chimp::EntityId TowerManager::PlaceTowerWithNetworkId(TowerType type, Chimp::Vector2f position, NetworkId id)
{
	auto entId = PlaceTower(type, position);
	auto networkId = m_ECS.GetMutableComponent<NetworkedIdentifierComponent>(entId);
	networkId->Id = id;
	return entId;
}

void TowerManager::RemoveTowerWithNetworkId(NetworkId id)
{
	auto view = m_ECS.GetEntitiesWithComponents<NetworkedIdentifierComponent, HealthComponent>();
	for (auto& [idComp, health] : view) {
		if (idComp.Id == id) {
			health.Health = 0;
			return;
		}
	}
}
