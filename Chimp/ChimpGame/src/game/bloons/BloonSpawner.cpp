#include "BloonSpawner.h"
#include "networking\Networking.h"

BloonSpawner::BloonSpawner(Chimp::Engine& engine,
	BloonManager& opponentBloonManager,
	MoneyManager& moneyManager,
	Chimp::TaskScheduler& taskScheduler) :
	m_Engine(engine),
	m_OpponentBloonManager(opponentBloonManager),
	m_BloonSendsDisabledTime(0),
	m_MoneyManager(moneyManager),
	m_TaskScheduler(taskScheduler)
{
}

void BloonSpawner::Update()
{
	if (m_BloonSendsDisabledTime > 0) {
		m_BloonSendsDisabledTime -= m_Engine.GetTimeManager().GetDeltaTime();
	}
}

void BloonSpawner::RenderUI()
{
	std::string name = "Bloons##";

	ImGui::SetCursorPos({ 75,75 });

	const bool bloonSendsDisabled = m_BloonSendsDisabledTime > 0;
	if (bloonSendsDisabled) {
		ImGui::BeginDisabled();
	}

#pragma region Bloon Buttons
	bool canNotAfford = !m_MoneyManager.HasMoney(20);
	if (canNotAfford) {
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Red x5 (-$20) (+$1/8s)")) {
		m_BloonSendsDisabledTime += 2.5f;
		m_MoneyManager.RemoveMoney(20);
		m_MoneyManager.AddToIncome(1.0f);
		for (size_t i = 0; i < 5; i++) {
			SendBloon(Bloons::BloonType::RED, i * 0.5f);
		}
	}
	if (canNotAfford) {
		ImGui::EndDisabled();
	}

	canNotAfford = !m_MoneyManager.HasMoney(30);
	if (canNotAfford) {
		ImGui::BeginDisabled();
	}
	ImGui::SetCursorPos({ 75, 100 });
	if (ImGui::Button("Blue x4 (-$30) (+$1.4/8s)")) {
		m_BloonSendsDisabledTime += 2.0f;
		m_MoneyManager.RemoveMoney(30);
		m_MoneyManager.AddToIncome(1.4f);
		for (size_t i = 0; i < 4; i++) {
			SendBloon(Bloons::BloonType::BLUE, i * 0.5f);
		}
	}
	if (canNotAfford) {
		ImGui::EndDisabled();
	}

	canNotAfford = !m_MoneyManager.HasMoney(50);
	if (canNotAfford) {
		ImGui::BeginDisabled();
	}
	ImGui::SetCursorPos({ 75, 125 });
	if (ImGui::Button("Green x3 (-$50) (+$1.8/8s)")) {
		m_MoneyManager.RemoveMoney(50);
		m_MoneyManager.AddToIncome(1.8f);
		m_BloonSendsDisabledTime += 1.5f;
		for (size_t i = 0; i < 3; i++) {
			SendBloon(Bloons::BloonType::GREEN, i * 0.5f);
		}
	}
	if (canNotAfford) {
		ImGui::EndDisabled();
	}

	canNotAfford = !m_MoneyManager.HasMoney(200);
	if (canNotAfford) {
		ImGui::BeginDisabled();
	}
	ImGui::SetCursorPos({ 75, 150 });
	if (ImGui::Button("XMAS (-$200) (-$1/8s)")) {
		m_BloonSendsDisabledTime += 4.0f;
		m_MoneyManager.RemoveMoney(200);
		m_MoneyManager.RemoveFromIncome(1.0f);
		for (size_t i = 0; i < 5; i++) {
			SendBloon(Bloons::BloonType::RED, i + 0.25f);
			SendBloon(Bloons::BloonType::GREEN, i + 0.5f);
			SendBloon(Bloons::BloonType::RED, i + 0.75f);
		}
	}
	if (canNotAfford) {
		ImGui::EndDisabled();
	}
#pragma endregion

	if (bloonSendsDisabled) {
		ImGui::EndDisabled();
	}
}

void BloonSpawner::SendBloon(Bloons::BloonType bloonType, float delay)
{
	m_TaskScheduler.RunDelayedTask(delay, [this, bloonType]() {
		m_OpponentBloonManager.SpawnBloon(bloonType);

		ClientSpawnBloonPacket packet;
		packet.PacketType = Networking::CLIENT_BLOON_SEND;
		packet.BloonType = bloonType;

#ifndef DEBUG_AUTOSTART_WITH_1_PLAYER
		const auto opponentId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetOpponentId();
		Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
#endif
		});
}
