#pragma once

#include "TowerTypes.h"

struct TowerComponent {
	TowerType Type;
	Chimp::InPlaceOptional<Chimp::EntityId> Target;
};