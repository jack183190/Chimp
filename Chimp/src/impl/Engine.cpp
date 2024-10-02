#pragma once

#include "api/Engine.h"

namespace Chimp {
	TimeManager& Engine::GetTimeManager()
	{
		return m_TimeManager;
	}
}