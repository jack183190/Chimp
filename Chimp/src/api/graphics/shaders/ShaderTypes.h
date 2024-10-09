#pragma once

#include "stdafx.h"
#include "api/utils/FileReader.h"

namespace Chimp {
	enum class ShaderType {
		VERTEX,
		FRAGMENT
	};

	struct ShaderSources {
		std::string vertexSource;
		std::string fragmentSource;
	};

	struct ShaderFilePaths {
		std::string vertexPath;
		std::string fragmentPath;

		ShaderSources GetSources() const {
			return {
				FileReader::Read(vertexPath),
				FileReader::Read(fragmentPath)
			};
		}
	};
}