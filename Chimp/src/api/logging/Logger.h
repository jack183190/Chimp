#pragma once

#include "stdafx.h"

namespace Chimp {
	enum class LogLevel {
		INFO = 0,
		WARNING = 1,
		ERROR = 2
	};

	class Logger {
	public:
		Logger(const std::string& name, LogLevel level = LogLevel::INFO);
		~Logger();

		void Info(const std::string& message);
		void Info(const char* message);
		void Info(const std::stringstream &message);

		void Warning(const std::string& message);
		void Warning(const char* message);
		void Warning(const std::stringstream& message);

		void Error(const std::string& message);
		void Error(const char* message);
		void Error(const std::stringstream& message);

		void Log(LogLevel level, const std::string& message);

		void SetLogToFile(bool logToFile);
		void SetLogToConsole(bool logToConsole);

	private:
		std::string m_Name;
		LogLevel m_Level;
		std::unique_ptr<std::ofstream> m_File;
		bool m_LogToFile;
		bool m_LogToConsole;
	};
}