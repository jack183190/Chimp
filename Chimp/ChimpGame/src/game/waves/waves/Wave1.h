#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave1 : public GameWave
	{
	public:
		Wave1(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			Delay(1.0f);
			MultipleBloons(Bloons::BloonType::RED, 0.25f, 4);
			Delay(1.0f);
			SingleBloon(Bloons::BloonType::BLUE);
			Delay(0.25f);
			SingleBloon(Bloons::BloonType::GREEN);
		}
	};
}