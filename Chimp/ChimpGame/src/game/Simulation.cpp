#include "Simulation.h"

Simulation::Simulation(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer,
	Chimp::Vector2f position)
	: m_Engine(engine),
	m_GameRenderer(gameRenderer),
	m_Position(position),
	m_Path({
		{ 60.0f, 0.0f },
		{ 60.0f, 140.0f },
		{ 540.0f, 140.0f },
		{ 540.0f, 450.0f },
		{ 280.0f, 450.0f },
		{ 280.0f, 720.0f }
		})
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

	testEntity = Entities::CreateBaseEntity(
		m_ECS,
		m_Engine.GetAssetManager().GetMesh("MapBackground"),
		{
			{ m_Position.x, m_Position.y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 10, 10, 1.0f}
		}
	);
}

void Simulation::Update()
{
	float dt = m_Engine.GetTimeManager().GetDeltaTime();
	dist += dt * 50.0f;
	auto [point, before, after] = m_Path.GetPointByDistance(dist);
	//if (before) std::cout << "before\n";
	//if (after) std::cout << "after\n";
	//if (after) dist = 0;
	m_ECS.GetMutableComponent<Chimp::TransformComponent>(testEntity)->SetTranslation(point.x, point.y, 0.0f);
	std::cout << "point: " << point.x << ", " << point.y << std::endl;
}

void Simulation::Render()
{
	m_GameRenderer->RenderWorld(m_ECS);
}
