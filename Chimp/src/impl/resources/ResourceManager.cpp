#include "api/resources/ResourceManager.h"
#include "api/Engine.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "Loggers.h"
#ifdef CHIMP_ASSIMP
#include "impl/resources/assimp/ModelImporter.h"
#endif

namespace Chimp {
	ResourceManager::ResourceManager(Engine& engine)
		: m_Engine(engine),
		m_MeshStorage(engine, *this),
		m_Shaders(engine),
		m_Textures(engine)
	{
	}

	ResourceContainer<ShaderFilePaths, IShader>& ResourceManager::GetShaders()
	{
		return m_Shaders;
	}

	ResourceContainer<std::string, ITexture>& ResourceManager::GetTextures()
	{
		return m_Textures;
	}

	Mesh& ResourceManager::LoadModel(const std::string& path, const IModelImporter::Settings& settings)
	{
		assert(m_ModelImporter);
		auto it = m_Models.find(path);
		if (it != m_Models.end())
		{
			return *it->second;
		}

		auto mesh = m_ModelImporter->LoadModel(path, settings);
		if (!mesh)
		{
			Loggers::Resources().Error("Failed to load model: " + path);
		}
		m_Models[path] = std::move(mesh);
		return *m_Models[path];
	}

	Mesh& ResourceManager::GetModel(const std::string& path)
	{
		auto it = m_Models.find(path);
		if (it == m_Models.end())
		{
			Loggers::Resources().Error("Model not loaded: " + path);
		}
		return *it->second;
	}

	void ResourceManager::UnloadModel(const std::string& path)
	{
		auto it = m_Models.find(path);
		if (it != m_Models.end())
		{
			m_Models.erase(it);
		}
	}

	MeshStorage& ResourceManager::GetMeshStorage()
	{
		return m_MeshStorage;
	}

	void ResourceManager::InitModelImporter()
	{
#ifdef CHIMP_ASSIMP
		m_ModelImporter = std::unique_ptr<ModelImporter>(new ModelImporter(m_Engine.GetRenderingManager(), *this));
#else
		Loggers::Resources().Error("No model importer available, can't load models.");
#endif
	}

	void ResourceManager::UnloadUnused()
	{
		m_Shaders.UnloadUnused();
	}
}