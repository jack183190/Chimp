#pragma once

#include "stdafx.h"

namespace Chimp {
	class FileReader {
		FileReader() = delete;
	public:
		static std::string Read(const std::string& path);
		static std::string Read(const std::filesystem::path& path);
	};
}