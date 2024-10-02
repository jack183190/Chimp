#pragma once

#include "api/time/TimeManager.h"

namespace Chimp {
	class EntryPoint;
	class Engine {
		friend class EntryPoint;
	private:
		Engine() = default;

	public:
		[[nodiscard]] TimeManager& GetTimeManager();

	private:
		TimeManager m_TimeManager;
	};
}