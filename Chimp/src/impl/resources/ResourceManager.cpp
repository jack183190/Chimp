#include "api/resources/ResourceManager.h"
#include "api/Engine.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "Loggers.h"
#ifdef CHIMP_ASSIMP
#include "impl/resources/assimp/ModelImporter.h"
#endif

namespace Chimp {
	ResourceManager::ResourceManager(Engine& engine)
		: m_Engine(engine)
	{
	}

	std::shared_ptr<IShader> ResourceManager::LoadShader(const ShaderFilePaths& shaderFilePaths)
	{
		auto it = m_Shaders.find(shaderFilePaths);
		if (it != m_Shaders.end())
		{
			return it->second;
		}

		std::shared_ptr<IShader> shader = m_Engine.GetRenderingManager().CompileShader(shaderFilePaths);
		if (!shader->IsValid())
		{
			std::stringstream ss;
			ss << "Failed to compile shader" << std::endl
				<< "Vertex: " << shaderFilePaths.Vertex << std::endl
				<< "Fragment: " << shaderFilePaths.Fragment << std::endl;
			Loggers::Resources().Error(ss.str());
		}
		m_Shaders[shaderFilePaths] = shader;
		return shader;
	}

	ITexture& ResourceManager::LoadTexture(const std::string& path)
	{
		auto it = m_Textures.find(path);
		if (it != m_Textures.end())
		{
			return *it->second;
		}

		std::unique_ptr<ITexture> texture = m_Engine.GetRenderingManager().CreateTextureFromImage(path);
		if (!texture)
		{
			Loggers::Resources().Error("Failed to load texture: " + path);
		}
		m_Textures[path] = std::move(texture);
		return *m_Textures[path];
	}

	void ResourceManager::UnloadTexture(const std::string& path)
	{
		auto it = m_Textures.find(path);
		if (it != m_Textures.end())
		{
			m_Textures.erase(it);
		}
	}

	void ResourceManager::StoreMesh(const std::string& id, std::unique_ptr<Mesh> mesh)
	{
		if (m_Meshes.find(id) != m_Meshes.end())
		{
			Loggers::Resources().Error("Mesh already exists: " + id);
			return;
		}
		m_Meshes[id] = std::move(mesh);
	}

	void ResourceManager::CreateTexturedQuad(const std::string& id, const std::string& texturePath)
	{
		CreateTexturedQuad(id, LoadTexture(texturePath));
	}

	void ResourceManager::CreateTexturedQuad(const std::string& id, ITexture& texture)
	{
		auto mesh = TexturedQuad::Create(
			m_Engine.GetRenderingManager(),
			texture
		);
		StoreMesh(id, std::move(mesh));
	}

	Mesh& ResourceManager::GetMesh(const std::string& id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end());
		return *it->second;
	}

	bool ResourceManager::HasMesh(const std::string& id) const
	{
		return m_Meshes.find(id) != m_Meshes.end();
	}

	std::unique_ptr<Mesh> ResourceManager::ReclaimStoredMesh(const std::string& id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end());
		std::unique_ptr<Mesh> mesh = std::move(it->second);
		m_Meshes.erase(it);
		return std::move(mesh);
	}

	Mesh& ResourceManager::LoadModel(const std::string& path, const IModelImporter::Settings& settings)
	{
		assert(m_ModelImporter);
		auto it = m_Models.find(path);
		if (it != m_Models.end())
		{
			return *it->second->Mesh;
		}

		std::unique_ptr<IModelImporter::ImportedMesh> importedMesh = m_ModelImporter->LoadModel(path, settings);
		if (!importedMesh)
		{
			Loggers::Resources().Error("Failed to load model: " + path);
		}
		m_Models[path] = std::move(importedMesh);
		return *m_Models[path]->Mesh;
	}

	void ResourceManager::UnloadModel(const std::string& path)
	{
		auto it = m_Models.find(path);
		if (it != m_Models.end())
		{
			m_Models.erase(it);
		}
	}
	void ResourceManager::InitModelImporter()
	{
#ifdef CHIMP_ASSIMP
		m_ModelImporter = std::unique_ptr<ModelImporter>(new ModelImporter(m_Engine.GetRenderingManager()));
#else
		Loggers::Resources().Error("No model importer available, can't load models.");
#endif
	}
}