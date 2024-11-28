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
		m_Textures(engine),
		m_Models(engine, *m_ModelImporter) // Technically bad but its ok trust me
	{
		InitModelImporter();
	}

	ResourceContainer<ShaderFilePaths, IShader>& ResourceManager::GetShaders()
	{
		return m_Shaders;
	}

	ResourceContainer<std::string, ITexture>& ResourceManager::GetTextures()
	{
		return m_Textures;
	}

	ModelResourceContainer& ResourceManager::GetModels()
	{
		assert(m_ModelImporter != nullptr);
		assert(m_Models.m_ModelImporter.HasValue());
		return m_Models;
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
		m_Models.m_ModelImporter = m_ModelImporter.get();
	}

	void ResourceManager::UnloadUnusedResources()
	{
		m_Shaders.UnloadUnused();
	}
}