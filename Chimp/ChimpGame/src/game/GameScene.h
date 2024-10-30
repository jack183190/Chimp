#pragma once

#include "stdafx.h"
#include "GameRenderer.h"

class GameScene : public Chimp::Scene {
public:
	GameScene(Chimp::Engine& engine, std::shared_ptr<GameRenderer> gameRenderer);
	~GameScene();

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<GameRenderer> m_GameRenderer;
};