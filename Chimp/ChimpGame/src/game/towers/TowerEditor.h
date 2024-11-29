#pragma once

#include "TowerManager.h"
#include "systems/TowerSelectionSystem.h"
#include "game/MoneyManager.h"

class TowerEditor {
public:
	TowerEditor(TowerManager& towerManager,
		Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition,
		Chimp::Vector2f simulationSize,
		MoneyManager& moneyManager);

	void Update();
	void RenderUI();

private:
	void Place(TowerType type, Chimp::Vector2f position);
	void RemoveSelectedTower();
	void UpgradeSelectedTower(UpgradeType type);

private:
	TowerManager& m_TowerManager;
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
	Chimp::Vector2f m_SimulationPosition;
	Chimp::Vector2f m_SimulationSize;
	bool m_IsPlacing = false;
	TowerType m_PlacingType;
	TowerSelectionSystem m_SelectionSystem;
	MoneyManager &m_MoneyManager;
	Chimp::TextureDependency m_DeselectTexture;
	std::vector<Chimp::TextureDependency> m_TowerIconTextures;
};