#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/utils/OptionalReference.h"
#include "IModelImporter.h"
#include "MeshStorage.h"

namespace Chimp {
	class Engine;
	class IShader;

	class ResourceManager {
		friend class Engine;
	private:
		ResourceManager(Engine& engine);

	public:
		// Load a shader, if it's already loaded, it will return the existing shader
		[[nodiscard]] std::shared_ptr<IShader> LoadShader(const ShaderFilePaths& shaderFilePaths);

		// Load a texture, if it's already loaded, it will return the existing texture
		[[nodiscard]] ITexture& LoadTexture(const std::string& path);
		void UnloadTexture(const std::string& path);

		// Load a model from a file, won't load same model twice, loads associated assets, see IModelImporter for more info
		Mesh& LoadModel(const std::string& path, const IModelImporter::Settings& settings = {});
		[[nodiscard]] Mesh& GetModel(const std::string& path);
		// Unload a model, this will unload all associated assets
		void UnloadModel(const std::string& path);

		// Get the mesh storage, used to store meshes/models that weren't loaded from file (see MeshStorage for more information)
		[[nodiscard]] MeshStorage& GetMeshStorage();

	private:
		void InitModelImporter();

	private:
		Engine& m_Engine;
		std::unique_ptr<IModelImporter> m_ModelImporter;
		MeshStorage m_MeshStorage;

		// Shaders are slow to compile but aren't expensive to store, so we'll just store them forever
		std::unordered_map<ShaderFilePaths, std::shared_ptr<IShader>> m_Shaders;

		std::unordered_map<std::string, std::unique_ptr<ITexture>> m_Textures;

		std::unordered_map<std::string, std::unique_ptr<IModelImporter::ImportedMesh>> m_Models;
	};
}