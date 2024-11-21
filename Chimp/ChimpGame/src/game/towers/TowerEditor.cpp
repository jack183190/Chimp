#include "TowerEditor.h"
#include "networking/Networking.h"

TowerEditor::TowerEditor(TowerManager& towerManager,
	Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition,
	Chimp::Vector2f simulationSize) :
	m_TowerManager(towerManager),
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition),
	m_SimulationSize(simulationSize)
{
}

void TowerEditor::Update()
{
	auto mousePos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
	if (m_Engine.GetWindow().GetInputManager().IsMouseButtonPressed(Chimp::Mouse::LEFT)
		&& m_IsPlacing
		&& Chimp::IsGreaterThanOrEqual(mousePos, m_SimulationPosition)
		&& Chimp::IsLessThanOrEqual(mousePos, m_SimulationPosition + m_SimulationSize)) {
		auto towerPos = Chimp::ComponentMax({ 0,0 }, mousePos - m_SimulationPosition);
		towerPos.y *= -1;
		Place(m_PlacingType, towerPos);
		m_IsPlacing = false;
	}
}

void TowerEditor::RenderUI()
{
	ImVec2 iconPosition = ImGui::GetWindowSize();
	iconPosition.x /= 2;
	iconPosition.x -= 60.0f * NUM_TOWERS / 2.0f;
	iconPosition.y -= 100;

	// DESELECT BUTTON
	if (m_IsPlacing) {
		ImGui::SetCursorPos({iconPosition.x - 100, iconPosition.y});
		intptr_t deselect = (intptr_t)m_Engine.GetResourceManager().LoadTexture(GAME_SRC + std::string("/assets/textures/X.png")).GetId();
		if (ImGui::ImageButton("##close", deselect, ImVec2(50, 50))) {
			m_IsPlacing = false;
		}
	}

	for (TowerType type = 0; type < NUM_TOWERS; type++) {
		intptr_t icon = (intptr_t)m_Engine.GetResourceManager().LoadTexture(TOWER_ICONS[type]).GetId();

		// DRAW SELECT BUTTON
		ImGui::SetCursorPos(iconPosition);
		std::string label = "##tower" + std::to_string(type);
		if (ImGui::ImageButton(label.c_str(), icon, ImVec2(50, 50))) {
			m_IsPlacing = true;
			m_PlacingType = type;
		}

		iconPosition.x += 60.0f;

		// DRAW PREVIEW PLACE
		if (m_IsPlacing && m_PlacingType == type) {
			auto mousePos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
			ImGui::SetCursorPos({ mousePos.x, mousePos.y });
			ImGui::Image(icon, ImVec2(50, 50));
		}
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
#ifndef DEBUG_AUTOSTART_WITH_1_PLAYER
	Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
#endif
}
