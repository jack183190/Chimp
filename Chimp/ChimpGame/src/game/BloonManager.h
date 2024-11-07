#pragma once

#include "Entities.h"

class BloonManager {
public:
	BloonManager(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f simulationPosition);

	void Update();
	void RenderUI();

	void SpawnBloon(Bloons::BloonType type);

	bool HasLost() const;

private:
	void HandleMovement(float dt);

private:
	Chimp::ECS& m_ECS;
	Chimp::Engine& m_Engine;
	Chimp::Path<Chimp::Vector2f> m_Path;
	Chimp::Vector2f m_SimulationPosition;
	static constexpr unsigned int MAX_LIVES = 10;
	unsigned int m_Lives = MAX_LIVES;
};