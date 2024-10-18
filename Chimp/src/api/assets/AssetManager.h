#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"

namespace Chimp {
	class Engine;
	class IShader;

	class AssetManager {
		friend class Engine;
	private:
		AssetManager(Engine& engine);

	public:
		[[nodiscard]] std::shared_ptr<IShader> LoadShader(const ShaderFilePaths& shaderFilePaths);

		[[nodiscard]] ITexture& LoadTexture(const std::string& path);
		void UnloadTexture(const std::string& path);

	private:
		Engine& m_Engine;

		// Shaders are slow to compile but aren't expensive to store, so we'll just store them forever
		std::unordered_map<ShaderFilePaths, std::shared_ptr<IShader>> m_Shaders;

		std::unordered_map<std::string, std::unique_ptr<ITexture>> m_Textures;
	};
}