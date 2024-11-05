#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/utils/OptionalReference.h"

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
		// Reclaim a stored mesh, this will remove it from the asset manager
		std::unique_ptr<Mesh> ReclaimStoredMesh(const std::string& id);
		std::unique_ptr<Mesh> DestroyStoredMesh(const std::string& id) { return ReclaimStoredMesh(id); }

	private:
		Engine& m_Engine;

		// Shaders are slow to compile but aren't expensive to store, so we'll just store them forever
		std::unordered_map<ShaderFilePaths, std::shared_ptr<IShader>> m_Shaders;

		std::unordered_map<std::string, std::unique_ptr<ITexture>> m_Textures;

		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_Meshes;
	};
}