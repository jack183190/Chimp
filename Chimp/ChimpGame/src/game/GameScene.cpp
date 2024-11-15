#include "GameScene.h"
#include "Entities.h"
#include "networking/Networking.h"

GameScene::GameScene(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer) :
	m_Engine(engine),
	m_GameRenderer(gameRenderer)
{
	m_LoadedSprites.push_back(GameRenderer::LoadSprite(m_Engine, "MapBackground", "MapBackground.png"));
	for (size_t i = 0; i < Bloons::NUM_BLOON_TYPES; ++i) {
		m_LoadedSprites.push_back(GameRenderer::LoadSprite(m_Engine, Bloons::BloonIds[i], Bloons::TexturePaths[i]));
	}

	m_OpponentSimulation = std::make_unique<Simulation>(engine, gameRenderer, Chimp::Vector2f{ 0.0f, 0.0f });
	m_PlayerSimulation = std::make_unique<Simulation>(engine, gameRenderer, Chimp::Vector2f{ m_Engine.GetWindow().GetSize().x / 2.0f, 0.0f });

	m_WaveStartHandler = std::make_unique<WaveStartHandler>(m_PlayerSimulation->GetWaveManager(), m_OpponentSimulation->GetWaveManager());

	m_MatchWinLoseHandler = std::make_unique<MatchWinLoseHandler>(m_Engine, *m_PlayerSimulation, m_GameRenderer);

	LoadModels();
}

GameScene::~GameScene()
{
	GameRenderer::UnloadSprites(m_Engine, m_LoadedSprites);
	UnloadModels();
}

void GameScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
}

void GameScene::OnDeactivate()
{
}

void GameScene::OnUpdate()
{
	auto& clientHandlers = Networking::GetClient()->GetHandlers();

	m_PlayerSimulation->Update();
	m_OpponentSimulation->Update();

	m_WaveStartHandler->Update();

	m_MatchWinLoseHandler->Update();

	if (m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::T)) {
		// place test tower
		m_PlayerSimulation->GetTowerManager().PlaceTower(TowerType::MONKEY, { 100, -250 });
	}
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
	ImVec2 windowCenter = { m_Engine.GetWindow().GetSize().x / 2.0f, m_Engine.GetWindow().GetSize().y / 2.0f };
	m_PlayerSimulation->RenderUI();
	m_OpponentSimulation->RenderUI();

	ImGui::SetCursorPos({ windowCenter.x - 100.0f, 50.0f });
	ImGui::SetWindowFontScale(2.5f);
	ImGui::Text("Wave: %d", m_PlayerSimulation->GetWaveManager().GetWave());
	ImGui::SetWindowFontScale(1.0f);
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

	m_Engine.GetResourceManager().LoadModel(std::string(GAME_SRC) + "/assets/models/monkey/MonkeyOBJ.obj", settings);
}

void GameScene::UnloadModels()
{
}
