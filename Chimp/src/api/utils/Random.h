#pragma once

#include "stdafx.h"

namespace Chimp {
	class Random {
	public:
		Random(std::random_device::result_type seed = std::random_device()())
			: m_RandomEngine(seed) {}

		// Returns a random int between min (inclusive) and max (exclusive)
		[[nodiscard]] int GetRandomInt(int min, int max) {
			assert(min <= max);
			return std::uniform_int_distribution<int>(min, max)(m_RandomEngine);
		}

		// Returns a random float between min (inclusive) and max (exclusive)
		[[nodiscard]] float GetRandomFloat(float min, float max) {
			assert(min <= max);
			return std::uniform_real_distribution<float>(min, max)(m_RandomEngine);
		}

	private:
		std::mt19937 m_RandomEngine;
	};
}