#include "TowerEditor.h"
#include "networking/Networking.h"

TowerEditor::TowerEditor(TowerManager& towerManager,
	Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition) :
	m_TowerManager(towerManager),
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition)
{
}

void TowerEditor::Update()
{
	auto mousePos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
	if (m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::T)) {
		// place test tower
		auto towerPos = Chimp::ComponentMax({ 0,0 }, mousePos - m_SimulationPosition);
		towerPos.y *= -1;
		Place(TowerType::MONKEY, towerPos);
	}
}

void TowerEditor::Place(TowerType type, Chimp::Vector2f position)
{
	m_TowerManager.PlaceTower(type, position);
	ClientTowerPlacePacket packet;
	packet.PacketType = Networking::CLIENT_TOWER_PLACE;
	packet.Position = position;
	packet.Type = type;

	const auto opponentId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetOpponentId();
	Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
}
