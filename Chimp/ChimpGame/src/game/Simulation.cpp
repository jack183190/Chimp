#include "Simulation.h"

Simulation::Simulation(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer,
	Chimp::Vector2f position)
	: m_Engine(engine),
	m_GameRenderer(gameRenderer),
	m_Position(position)
{
	Entities::CreateBaseEntity(
		m_ECS,
		m_Engine.GetAssetManager().GetMesh("MapBackground"),
		{
			{ m_Position.x, m_Position.y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ m_Engine.GetWindow().GetSize().x / 2.0f, m_Engine.GetWindow().GetSize().y, 1.0f}
		}
	);
}

void Simulation::Update()
{
}

void Simulation::Render()
{
	m_GameRenderer->RenderWorld(m_ECS);
}
