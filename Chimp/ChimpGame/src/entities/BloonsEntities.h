#pragma once

#include "stdafx.h"
#include "BaseEntity.h"
#include "health/HealthComponent.h"
#include "EntityIdComponent.h"
#include "bloons/MoveableComponent.h"

namespace Bloons {
	constexpr size_t NUM_BLOON_TYPES = 3;
	enum class BloonType {
		RED = 0, 
		BLUE = 1,
		GREEN = 2
	};
	const std::array<std::string, NUM_BLOON_TYPES> BloonIds = {
		"Red",
		"Blue",
		"Green"
	};
	const std::array<std::string, NUM_BLOON_TYPES> TexturePaths = {
		"Bloons/Red.png",
		"Bloons/Blue.png",
		"Bloons/Green.png"
	};
	const std::array<int, NUM_BLOON_TYPES> HealthValues = {
		1,
		2,
		3
	};
	const std::array<float, NUM_BLOON_TYPES> SpeedValues = {
		75.0f,
		100.0f,
		150.0f
	};
	constexpr BloonType HealthToBloonType(int health) {
		if (health == 1) {
			return BloonType::RED;
		}
		else if (health == 2) {
			return BloonType::BLUE;
		}
		else {
			return BloonType::GREEN;
		}
	}

	void DamageBloon(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::EntityId bloonEntity, int damage);
}

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type);
}