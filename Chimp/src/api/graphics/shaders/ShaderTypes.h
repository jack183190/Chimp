#pragma once

#include "stdafx.h"
#include "api/utils/FileReader.h"

namespace Chimp {
	enum class ShaderType {
		VERTEX,
		FRAGMENT
	};

	struct ShaderFilePaths {
		std::string Vertex;
		std::string Fragment;

		std::unordered_map<ShaderType, std::string> AsMap() const {
			return {
				{ ShaderType::VERTEX, Vertex },
				{ ShaderType::FRAGMENT, Fragment }
			};
		}
	};
}