#include "BloonManager.h"
#include "Debug.h"

BloonManager::BloonManager(
	Chimp::Engine& engine, 
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition) :
	m_ECS(ecs),
	m_Engine(engine),
	m_Path({
		{ 310.0f, 0.0f },
		{ 310.0f, -210.0f },
		{ 50.0f, -210.0f },
		{ 50.0f, -530.0f },
		{ 530.0f, -530.0f },
		{ 530.0f, -720.0f }
		}, simulationPosition),
	m_SimulationPosition(simulationPosition)
{

}

void BloonManager::Update()
{
	if (m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::R)) {
		SpawnBloon(Bloons::BloonType::RED);
	}

	float dt = m_Engine.GetTimeManager().GetDeltaTime();
	HandleMovement(dt);
}

void BloonManager::RenderUI()
{
	ImGui::SetCursorPos({ m_SimulationPosition.x, m_SimulationPosition.y });
	std::string livesStr = "Lives: " + std::to_string(m_Lives);
	ImGui::ProgressBar((float)m_Lives / MAX_LIVES, { 200, 20 }, livesStr.c_str());
}

void BloonManager::SpawnBloon(Bloons::BloonType type)
{
	Entities::CreateBloonEntity(
		m_ECS,
		m_Engine,
		m_Path.GetStart(),
		type
	);
}

bool BloonManager::HasLost() const
{
	return m_Lives <= 0;
}

void BloonManager::HandleMovement(float dt)
{
	auto view = m_ECS.GetEntitiesWithComponents<MoveableComponent, HealthComponent, Chimp::TransformComponent>();
	dt *= DEBUG_BLOON_SPEED_MULTIPLIER;
	for (auto& [moveable, health, transform] : view) {
		moveable.DistanceTravelled += dt * moveable.Speed;

		// Update position
		auto [point, beforePath, afterPath] = m_Path.GetPointByDistance(moveable.DistanceTravelled);

		transform.SetTranslationXY(point);

		// Reached end of path?
		if (!afterPath) continue;

		m_Lives -= health.Health;
		health.Health = 0;
	}
}
