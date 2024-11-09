#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave3 : public GameWave
	{
	public:
		Wave3(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::GREEN, 0.25f, 5);
		}
	};
}