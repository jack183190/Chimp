#include "GameScene.h"
#include "Entities.h"
#include "gameover/GameOverScene.h"
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
}

GameScene::~GameScene()
{
	GameRenderer::UnloadSprites(m_Engine, m_LoadedSprites);
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

	// Did we lose?
	if (m_PlayerSimulation->HasLost()) {
		// Send win packet to opponent
		ClientMatchWinPacket packet = {};
		packet.PacketType = Networking::CLIENT_MATCH_WIN;
		packet.MatchId = clientHandlers.CurrentMatchHandler->GetMatchId();
		Networking::GetClient()->GetClient().SendPacketToClient(
			clientHandlers.CurrentMatchHandler->GetOpponentId(), 
			packet
		);

		// Send end packet to server
		ServerMatchEndPacket endPacket = {};	
		endPacket.PacketType = Networking::SERVER_MATCH_END;
		endPacket.MatchId = clientHandlers.CurrentMatchHandler->GetMatchId();
		Networking::GetClient()->GetClient().SendPacketToServer(endPacket);

		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameOverScene>(m_Engine, m_GameRenderer, false));
	}
	// Did we win?
	else if (clientHandlers.WinListener->DidWinMatch(clientHandlers.CurrentMatchHandler->GetMatchId())) {
		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameOverScene>(m_Engine, m_GameRenderer, true));
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
}

bool GameScene::ShouldExit(Chimp::Engine& engine) const
{
	return	m_Engine.GetWindow().GetInputManager().IsKeyDown(Chimp::Keyboard::ESCAPE);
}
