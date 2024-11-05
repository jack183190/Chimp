#include "GameScene.h"

GameScene::GameScene(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> gameRenderer) :
	m_Engine(engine),
	m_GameRenderer(gameRenderer)
{
	m_Engine.GetAssetManager().CreateTexturedQuad("MapBackground", std::string(GAME_SRC) + "/assets/textures/MapBackground.png");

	//m_OpponentSimulation = std::make_unique<Simulation>(engine, gameRenderer, Chimp::Vector2f{ 0.0f, 0.0f });
	m_PlayerSimulation = std::make_unique<Simulation>(engine, gameRenderer, Chimp::Vector2f{ m_Engine.GetWindow().GetSize().x / 2.0f, 0.0f });
}

GameScene::~GameScene()
{
	m_Engine.GetAssetManager().DestroyStoredMesh("MapBackground");
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
	//m_OpponentSimulation->Update();
}

void GameScene::OnRender()
{
	m_GameRenderer->BeginFrame();

	m_PlayerSimulation->Render();
	//m_OpponentSimulation->Render();
}

void GameScene::OnRenderUI()
{
}
