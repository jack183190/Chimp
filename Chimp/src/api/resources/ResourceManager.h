#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/utils/OptionalReference.h"
#include "IModelImporter.h"

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

		// Store a mesh, asset manager will keep it until its unloaded
		void StoreMesh(const std::string& id, std::unique_ptr<Mesh> mesh);
		// Create and store a textured quad mesh
		// id - id to store the mesh under
		// texturePath - path to the texture to use, will be loaded if it isn't already in the asset manager
		void CreateTexturedQuad(const std::string& id, const std::string& texturePath);
		// texture - texture to use
		void CreateTexturedQuad(const std::string& id, ITexture& texture);
		// Get mesh, bad stuff happens if you haven't stored it
		[[nodiscard]] Mesh& GetMesh(const std::string& id);
		// True if the mesh is stored
		[[nodiscard]] bool HasMesh(const std::string& id) const;
		// Reclaim a stored mesh, this will remove it from the asset manager
		std::unique_ptr<Mesh> ReclaimStoredMesh(const std::string& id);
		std::unique_ptr<Mesh> DestroyStoredMesh(const std::string& id) { return ReclaimStoredMesh(id); }

		// Load a model from a file, won't load same model twice, loads associated assets, see IModelImporter for more info
		[[nodiscard]] Mesh& LoadModel(const std::string& path, const IModelImporter::Settings& settings = {});
		// Unload a model, this will unload all associated assets
		void UnloadModel(const std::string& path);

	private:
		void InitModelImporter();

	private:
		Engine& m_Engine;

		std::unique_ptr<IModelImporter> m_ModelImporter;

		// Shaders are slow to compile but aren't expensive to store, so we'll just store them forever
		std::unordered_map<ShaderFilePaths, std::shared_ptr<IShader>> m_Shaders;

		std::unordered_map<std::string, std::unique_ptr<ITexture>> m_Textures;

		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_Meshes; // todo move into sub object

		std::unordered_map<std::string, std::unique_ptr<IModelImporter::ImportedMesh>> m_Models;
	};
}