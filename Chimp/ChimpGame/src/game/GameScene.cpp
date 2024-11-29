#include "GameScene.h"
#include "Entities.h"
#include "networking/Networking.h"

GameScene::GameScene(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer) :
	m_Engine(engine),
	m_GameRenderer(gameRenderer)
{
	auto& sprites = m_Engine.GetResourceManager().GetSprites();
	sprites.Depend(GAME_SRC + std::string("/assets/textures/Dart.png"));
	sprites.Depend(GAME_SRC + std::string("/assets/textures/MapBackground.png"));
	for (size_t i = 0; i < Bloons::NUM_BLOON_TYPES; ++i) {
		sprites.Depend(GAME_SRC + std::string("/assets/textures/") + Bloons::TexturePaths[i]);
	}

	const auto simulationSize = Chimp::ComponentMultiply(m_Engine.GetWindow().GetSize(), { 0.5, 1.0 });
	m_OpponentSimulation = std::make_unique<Simulation>(engine, gameRenderer, Chimp::Vector2f{ 0.0f, 0.0f }, simulationSize, false, m_MoneyManager);
	m_PlayerSimulation = std::make_unique<Simulation>(engine, gameRenderer, Chimp::Vector2f{ m_Engine.GetWindow().GetSize().x / 2.0f, 0.0f }, simulationSize, true, m_MoneyManager);

	m_WaveStartHandler = std::make_unique<WaveStartHandler>(m_PlayerSimulation->GetWaveManager(), m_OpponentSimulation->GetWaveManager());

	m_MatchWinLoseHandler = std::make_unique<MatchWinLoseHandler>(m_Engine, *m_PlayerSimulation, m_GameRenderer);

	m_BloonSpawner = std::make_unique<BloonSpawner>(m_Engine, m_OpponentSimulation->GetBloonManager(), m_MoneyManager);

	LoadModels();

	m_GameRunningTimer.Start();
}

GameScene::~GameScene()
{

	UnloadModels();
}

void GameScene::OnInit()
{
	m_PlayerSimulation->Init();
	m_OpponentSimulation->Init();
}

void GameScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
}

void GameScene::OnDeactivate()
{
}

void GameScene::OnUpdate()
{
	m_PlayerSimulation->Update();
	m_OpponentSimulation->Update();

	m_WaveStartHandler->Update();

	m_MatchWinLoseHandler->Update();

	m_BloonSpawner->Update();

	m_MoneyManager.Update();

	Networking::GetClient()->GetHandlers().BloonListener->Update(m_PlayerSimulation->GetBloonManager());
}

void GameScene::OnRender()
{
	m_GameRenderer->BeginFrame();

	m_PlayerSimulation->Render();
	m_OpponentSimulation->Render();
}

void GameScene::OnRenderUI()
{
	ImGui::Begin("##GameSceneUI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowPos({ 0.0f, 0.0f });
	ImGui::SetWindowSize({ m_Engine.GetWindow().GetSize().x, m_Engine.GetWindow().GetSize().y });

	m_PlayerSimulation->RenderUI();
	m_OpponentSimulation->RenderUI();

	ImGui::SetWindowFontScale(2.5f);
	ImGui::SetCursorPosY(100);
	m_Engine.GetImGuiHelper().CenteredTextHorizontally(std::format("Wave: {}", m_PlayerSimulation->GetWaveManager().GetWave()));
	ImGui::SetWindowFontScale(2.0f);
	m_Engine.GetImGuiHelper().CenteredTextHorizontally(m_Engine.GetTimeManager().FormatTime(m_GameRunningTimer.GetSecondsElapsed()));
	m_Engine.GetImGuiHelper().CenteredTextHorizontally(std::format("${} (+${}/{}s)", m_MoneyManager.GetMoney(), m_MoneyManager.GetIncome(), MoneyManager::IncomeInterval));
	ImGui::SetWindowFontScale(1.0f);

	m_BloonSpawner->RenderUI();
	ImGui::End();
}

bool GameScene::ShouldExit(Chimp::Engine& engine) const
{
	return m_Engine.GetWindow().GetInputManager().IsKeyDown(Chimp::Keyboard::ESCAPE)
		|| Scene::ShouldExit(engine);
}

void GameScene::LoadModels()
{
	Chimp::IModelImporter::Settings settings;
	settings.FlipUVs = false;
	settings.IncludeNormals = false;
	settings.IncludeTextureCoordinates = true;

	m_Engine.GetResourceManager().GetModels().ImportSettings = settings;
	m_Engine.GetResourceManager().GetModels().Depend(std::string(GAME_SRC) + "/assets/models/monkey/MonkeyOBJ.obj");
}

void GameScene::UnloadModels()
{
}
