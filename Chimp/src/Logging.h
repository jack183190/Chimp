#pragma once

#include "api/logging/Logger.h"

namespace Chimp::Loggers {
	static Logger& Main()
	{
		static Logger logger("Chimp", LogLevel::INFO);
		return logger;
	}
}