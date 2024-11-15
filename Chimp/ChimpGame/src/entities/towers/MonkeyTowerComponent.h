#pragma once

#include "stdafx.h"

struct MonkeyTowerComponent {
	bool Padding;
};

namespace Entities {
	Chimp::EntityId CreateMonkeyTower(Chimp::ECS& ecs, Chimp::Mesh& mesh, Chimp::Vector2f position);
}