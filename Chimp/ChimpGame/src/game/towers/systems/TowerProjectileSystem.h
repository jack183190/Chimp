#pragma once

#include "stdafx.h"
#include "game/Entities.h"

class TowerProjectileSystem
{
public:
	TowerProjectileSystem(Chimp::Engine& engine, Chimp::ECS& ecs);

	void Update();

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
};