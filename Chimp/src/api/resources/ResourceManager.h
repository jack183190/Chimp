#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/utils/OptionalReference.h"
#include "IModelImporter.h"
#include "MeshStorage.h"
#include "containers/ShaderResourceContainer.h"
#include "containers/TextureResourceContainer.h"


namespace Chimp {
	class Engine;
	class IShader;

	class ResourceManager {
		friend class Engine;
	private:
		ResourceManager(Engine& engine);

	public:
		[[nodiscard]] ResourceContainer<ShaderFilePaths, IShader>& GetShaders();

		[[nodiscard]] ResourceContainer<std::string, ITexture>& GetTextures();

		// Load a model from a file, won't load same model twice, loads associated assets, see IModelImporter for more info
		Mesh& LoadModel(const std::string& path, const IModelImporter::Settings& settings = {});
		[[nodiscard]] Mesh& GetModel(const std::string& path);
		// Unload a model, this will unload all associated assets
		void UnloadModel(const std::string& path);

		// Get the mesh storage, used to store meshes/models that weren't loaded from file (see MeshStorage for more information)
		[[nodiscard]] MeshStorage& GetMeshStorage();

		// Unload all assets with 0 references
		void UnloadUnused();

	private:
		void InitModelImporter();

	private:
		Engine& m_Engine;
		std::unique_ptr<IModelImporter> m_ModelImporter;
		MeshStorage m_MeshStorage;

		ShaderResourceContainer m_Shaders;
		TextureResourceContainer m_Textures;

		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_Models;
	};
}