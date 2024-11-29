#pragma once

#include "stdafx.h"
#include "rendering/GameRenderer.h"

class GameOverScene : public Chimp::Scene {
public:
	GameOverScene(Chimp::Engine& engine,
		std::shared_ptr<GameRenderer> gameRenderer,
		bool didWin);
	~GameOverScene();

	void OnInit() override;
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	bool m_DidWin;
};