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
#include "containers/ModelResourceContainer.h"
#include "containers/SpriteResourceContainer.h"

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

		[[nodiscard]] ModelResourceContainer& GetModels();

		[[nodiscard]] SpriteResourceContainer& GetSprites();

		// Get the mesh storage, used to store meshes/models that weren't loaded from file (see MeshStorage for more information)
		[[nodiscard]] MeshStorage& GetMeshStorage();

		// Unload all assets with 0 references
		void UnloadUnusedResources();

		// Load required assets
		void LoadRequiredResources();

	private:
		void InitModelImporter();

	private:
		Engine& m_Engine;
		std::unique_ptr<IModelImporter> m_ModelImporter;
		MeshStorage m_MeshStorage;

		ShaderResourceContainer m_Shaders;
		TextureResourceContainer m_Textures;
		ModelResourceContainer m_Models;
		SpriteResourceContainer m_Sprites;
	};
}