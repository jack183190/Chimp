#pragma once

#include "waves/Wave1.h"
#include "game/BloonManager.h"

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
			.Build();
	}
};