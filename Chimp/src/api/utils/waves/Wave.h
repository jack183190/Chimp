#pragma once

#include "WaveCommand.h"

namespace Chimp {

	class WaveManager;
	class Wave {
		friend class WaveManager;
	protected:
		Wave() {}
	public:
		virtual ~Wave() = default;

		// Update, return true if wave is finished
		bool Update(Engine& engine);

	protected:
		void Delay(float seconds);
		void RunFunction(std::function<void(Engine&)> function);
		void RunRepeatingFunction(std::function<void(Engine&)> function, float interval, unsigned int repeats);

	protected:
		std::queue<std::unique_ptr<WaveCommand>> m_Commands;

	private:
		float m_CommandElapsedTime = 0.0f;
	};
}