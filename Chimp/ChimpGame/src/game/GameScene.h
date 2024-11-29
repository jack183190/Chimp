#pragma once

#include "stdafx.h"
#include "rendering/GameRenderer.h"
#include "Simulation.h"
#include "waves/WaveStartHandler.h"
#include "match_end/MatchWinLoseHandler.h"
#include "bloons/BloonSpawner.h"
#include "MoneyManager.h"

class GameScene : public Chimp::Scene {
public:
	GameScene(Chimp::Engine& engine, std::shared_ptr<GameRenderer> gameRenderer);
	~GameScene();

	void OnInit() override;
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

	bool ShouldExit(Chimp::Engine& engine) const override;

private:
	void LoadModels();
	void UnloadModels();

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	std::unique_ptr<Simulation> m_OpponentSimulation;
	std::unique_ptr<Simulation> m_PlayerSimulation;
	std::unique_ptr<WaveStartHandler> m_WaveStartHandler;
	std::unique_ptr<MatchWinLoseHandler> m_MatchWinLoseHandler;
	std::unique_ptr<BloonSpawner> m_BloonSpawner;
	MoneyManager m_MoneyManager;
	Chimp::Timer m_GameRunningTimer;
};