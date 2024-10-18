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
			std::cerr << "Failed to compile shader" << std::endl;
			exit(-1);
		}
		m_Shaders[shaderFilePaths] = shader;
		return shader;
	}
}