#include "api/time/TimeManager.h"

namespace Chimp {
	constexpr float DEFAULT_DELTA_TIME = 1.0f / 60.0f; // 60 FPS

	TimeManager::TimeManager() 
		: m_DeltaTime(DEFAULT_DELTA_TIME),
	m_FrameStartTime(std::chrono::steady_clock::now()) {

	}

	void TimeManager::Update()
	{
		const auto frameEndTime = std::chrono::steady_clock::now();
		m_DeltaTime = std::chrono::duration<float>(frameEndTime - m_FrameStartTime).count();

		m_FrameStartTime = std::chrono::steady_clock::now();
	}

	float TimeManager::GetDeltaTime() const {
		return m_DeltaTime;
	}
}