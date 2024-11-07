#pragma once

#include "stdafx.h"
#include "BaseEntity.h"

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
}

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type);
}