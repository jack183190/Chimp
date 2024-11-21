#include "Simulation.h"
#include "networking/Networking.h"

Simulation::Simulation(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer,
	Chimp::Vector2f position,
	Chimp::Vector2f size,
	bool isPlayerSimulation)
	: m_Engine(engine),
	m_GameRenderer(gameRenderer),
	m_Position(position),
	m_Size(size),
	m_HealthSystem(m_ECS),
	m_BloonManager(m_Engine, m_ECS, m_Position),
	m_WaveManager(WaveManagerBuilder::Build(m_Engine, m_BloonManager)),
	m_TowerManager(m_Engine, m_ECS, m_Position),
	m_IsPlayerSimulation(isPlayerSimulation)
{
	if (isPlayerSimulation) {
		m_TowerEditor = std::make_unique<TowerEditor>(m_TowerManager, m_Engine, m_ECS, m_Position, m_Size);
	}

	Entities::CreateBaseEntity(
		m_ECS,
		m_Engine.GetResourceManager().GetMeshStorage().GetMesh("MapBackground"),
		{
			{ m_Position.x, m_Position.y, 10.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ m_Engine.GetWindow().GetSize().x / 2.0f, m_Engine.GetWindow().GetSize().y, 500.0f}
		}
	);
}

void Simulation::Update()
{
	m_HealthSystem.Update();
	m_BloonManager.Update();
	m_WaveManager->Update();
	m_TowerManager.Update();
	if (m_TowerEditor) {
		m_TowerEditor->Update();
	}
	if (!m_IsPlayerSimulation) {
		Networking::GetClient()->GetHandlers().TowerPlaceListener->Update(m_TowerManager);
	}
}

void Simulation::Render()
{
	m_GameRenderer->RenderWorld(m_ECS);
}

void Simulation::RenderUI()
{
	m_BloonManager.RenderUI();
	if (m_TowerEditor) {
		m_TowerEditor->RenderUI();
	}
}

bool Simulation::HasLost() const
{
	return m_BloonManager.HasLost() ||
#ifndef NDEBUG
		m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::L) || 
#endif
		!Networking::GetClient()->IsConnected();
}

Chimp::WaveManager& Simulation::GetWaveManager()
{
	return *m_WaveManager;
}

TowerManager& Simulation::GetTowerManager()
{
	return m_TowerManager;
}

Chimp::Vector2f Simulation::GetPosition() const
{
	return m_Position;
}
