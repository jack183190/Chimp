#pragma once

#include "stdafx.h"
#include "rendering/GameRenderer.h"
#include "Simulation.h"

class GameScene : public Chimp::Scene {
public:
	GameScene(Chimp::Engine& engine, std::shared_ptr<GameRenderer> gameRenderer);
	~GameScene();

	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	std::unique_ptr<Simulation> m_OpponentSimulation;
	std::unique_ptr<Simulation> m_PlayerSimulation;
};