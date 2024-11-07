#include "Simulation.h"
#include "networking/Networking.h"

Simulation::Simulation(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer,
	Chimp::Vector2f position)
	: m_Engine(engine),
	m_GameRenderer(gameRenderer),
	m_Position(position),
	m_HealthSystem(m_ECS),
	m_BloonManager(m_ECS, m_Engine, m_Position)
{
	Entities::CreateBaseEntity(
		m_ECS,
		m_Engine.GetAssetManager().GetMesh("MapBackground"),
		{
			{ m_Position.x, m_Position.y, 10.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ m_Engine.GetWindow().GetSize().x / 2.0f, m_Engine.GetWindow().GetSize().y, 1.0f}
		}
	);

	m_BloonManager.SpawnBloon(Bloons::BloonType::GREEN);
}

void Simulation::Update()
{
	m_HealthSystem.Update();
	m_BloonManager.Update();
}

void Simulation::Render()
{
	m_GameRenderer->RenderWorld(m_ECS);
}

void Simulation::RenderUI()
{
	m_BloonManager.RenderUI();
}

bool Simulation::HasLost() const
{
	return m_BloonManager.HasLost() ||
#ifndef NDEBUG
		m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::L) || 
#endif
		!Networking::GetClient()->IsConnected();
}
