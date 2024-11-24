#pragma once

#include "stdafx.h"

namespace Chimp {
	class Engine;
	class MainLoop;
	class TimeManager {
		friend class Engine;
		friend class MainLoop;
	private:
		TimeManager();

	private:
		void Update();

	public:
		float GetDeltaTime() const;
		float GetSecondsRunning() const;

	private:
		std::chrono::steady_clock::time_point m_FrameStartTime;
		float m_DeltaTime;
		float m_SecondsRunning;
	};
}