#include "api/assets/AssetManager.h"
#include "api/Engine.h"

namespace Chimp {
	AssetManager::AssetManager(Engine& engine)
		: m_Engine(engine)
	{
	}

	std::shared_ptr<IShader> AssetManager::LoadShader(const ShaderFilePaths& shaderFilePaths)
	{
		auto it = m_Shaders.find(shaderFilePaths);
		if (it != m_Shaders.end())
		{
			return it->second;
		}

		std::shared_ptr<IShader> shader = m_Engine.GetRenderingManager().CompileShader(shaderFilePaths);
		if (!shader->IsValid())
		{
			std::cerr << "Failed to compile shader" << std::endl
				<< "Vertex: " << shaderFilePaths.Vertex << std::endl
				<< "Fragment: " << shaderFilePaths.Fragment << std::endl;
		}
		m_Shaders[shaderFilePaths] = shader;
		return shader;
	}

	ITexture& AssetManager::LoadTexture(const std::string& path)
	{
		auto it = m_Textures.find(path);
		if (it != m_Textures.end())
		{
			return *it->second;
		}

		std::unique_ptr<ITexture> texture = m_Engine.GetRenderingManager().CreateTextureFromImage(path);
		if (!texture)
		{
			std::cerr << "Failed to load texture: " << path << std::endl;
		}
		m_Textures[path] = std::move(texture);
		return *m_Textures[path];
	}

	void AssetManager::UnloadTexture(const std::string& path)
	{
		auto it = m_Textures.find(path);
		if (it != m_Textures.end())
		{
			m_Textures.erase(it);
		}
	}
}