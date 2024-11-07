#pragma once

#include "stdafx.h"
#include "rendering/GameRenderer.h"
#include "Entities.h"

class Simulation {
public:
	Simulation(Chimp::Engine& engine, 
		std::shared_ptr<GameRenderer> gameRenderer,
		Chimp::Vector2f position);

	void Update();
	void Render();

	bool HasLost() const;
private:
	Chimp::Engine& m_Engine;
	Chimp::Vector2f m_Position;
	Chimp::ECS m_ECS;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	Chimp::Path<Chimp::Vector2f> m_Path;
	Chimp::EntityId testEntity;
	float dist = 0;
};