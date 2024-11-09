#pragma once

#include "game/BloonManager.h"

#include "waves/Wave1.h"
#include "waves/Wave2.h"
#include "waves/Wave3.h"

class WaveManagerBuilder
{
private:
	WaveManagerBuilder() = delete;

#define ADD_WAVE_TO_BUILDER(className) \
	.AddWave(std::make_unique<Waves::className>(bloonManager))
public:
	static std::unique_ptr<Chimp::WaveManager> Build(
		Chimp::Engine& engine,
		BloonManager& bloonManager)
	{
		return Chimp::WaveManager::Builder(engine)
			ADD_WAVE_TO_BUILDER(Wave1)
			ADD_WAVE_TO_BUILDER(Wave2)
			ADD_WAVE_TO_BUILDER(Wave3)
			.Build();
	}
};