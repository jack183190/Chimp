#include "TowerRotationSystem.h"

TowerRotationSystem::TowerRotationSystem(Chimp::Engine& engine,
	Chimp::ECS& ecs) :
	m_Engine(engine),
	m_ECS(ecs)
{
}

void TowerRotationSystem::Update()
{
	auto towersView = m_ECS.GetEntitiesWithComponents<TowerComponent, Chimp::TransformComponent>();

	for (auto& [tower, transform] : towersView) {
		if (tower.Target.Empty()) {
			continue;
		}

		const auto& bloonTransform = m_ECS.GetComponent<Chimp::TransformComponent>(tower.Target.Value());
		const auto bloonPos = bloonTransform->GetTranslation()
			+ Chimp::Dot({ 0.5, -0.5, 0 }, bloonTransform->GetScale());
		const auto& towerPos = transform.GetTranslation()
			+ Chimp::Dot({ 0.5, -0.5, 0 }, transform.GetScale());
		const auto angle = Chimp::Atan2({ bloonPos.x - towerPos.x, bloonPos.y - towerPos.y });
		transform.SetRoll(angle - Chimp::PI / 2);
	}
}
