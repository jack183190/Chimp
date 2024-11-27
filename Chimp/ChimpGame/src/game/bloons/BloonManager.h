#pragma once

#include "game/Entities.h"
#include "Debug.h"

class BloonManager {
public:
	BloonManager(Chimp::Engine& engine, Chimp::ECS& ecs, Chimp::Vector2f simulationPosition);

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
	static constexpr unsigned int StartingLives =
#ifdef DEBUG_LOTS_OF_LIVES
		10000;
#else
		100;
#endif
	unsigned int m_Lives = StartingLives;
};