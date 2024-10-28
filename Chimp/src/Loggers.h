#pragma once

#include "api/logging/Logger.h"

namespace Chimp::Loggers {
	static Logger& Main()
	{
		static Logger logger("ChimpMain", LogLevel::INFO);
		return logger;
	}

	static Logger& Network()
	{
		static Logger logger("ChimpNetwork", LogLevel::WARNING);
		return logger;
	}

	static Logger& Resources()
	{
		static Logger logger("ChimpResources", LogLevel::WARNING);
		return logger;
	}
}