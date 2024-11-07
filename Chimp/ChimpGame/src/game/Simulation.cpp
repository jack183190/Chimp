#include "Simulation.h"

Simulation::Simulation(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer,
	Chimp::Vector2f position)
	: m_Engine(engine),
	m_GameRenderer(gameRenderer),
	m_Position(position),
	m_Path({
		{ 310.0f, 0.0f },
		{ 310.0f, -210.0f },
		{ 50.0f, -210.0f },
		{ 50.0f, -530.0f },
		{ 530.0f, -530.0f },
		{ 530.0f, -720.0f }
		})
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

	testEntity = Entities::CreateBloonEntity(
		m_ECS,
		m_Engine,
		{ 0.0f, 0.0f },
		Bloons::BloonType::BLUE
	);
}

void Simulation::Update()
{
	float dt = m_Engine.GetTimeManager().GetDeltaTime();
	dist += dt * 150.0f;
	auto [point, before, after] = m_Path.GetPointByDistance(dist);
	if (after) dist = 0;
	auto mut = m_ECS.GetMutableComponent<Chimp::TransformComponent>(testEntity);
	mut->SetTranslation({ point.x + m_Position.x, point.y + m_Position.y, 0.0f });
}

void Simulation::Render()
{
	m_GameRenderer->RenderWorld(m_ECS);
}

bool Simulation::HasLost() const
{
	return m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::L);
}
