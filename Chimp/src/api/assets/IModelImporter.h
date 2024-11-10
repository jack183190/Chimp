#pragma once

#include "stdafx.h"
#include "api/graphics/meshes/Mesh.h"

namespace Chimp {
	class AssetManager;
	class IRenderingManager;
	class IModelImporter {
	public:
		// Represents a mesh and textures associated with it
		// the mesh and textures are owned by this struct
		struct ImportedMesh {
			std::unique_ptr<Mesh> Mesh;
			std::vector<std::unique_ptr<ITexture>> AssociatedTextures;
		};

		struct Settings {
			bool FlipUVs = false;
			bool IncludeNormals = false;
			bool IncludeTextureCoordinates = true;
		};
	protected:
		DISABLE_COPY(IModelImporter);
		IModelImporter(IRenderingManager& renderingManager) :
			m_RenderingManager(renderingManager) {}
	public:
		virtual ~IModelImporter() = default;

		// Load a model from a file, this model can contain multiple meshes with 1 texture per mesh.
		// path - The path to the model file
		// Returns a unique pointer to an ImportedMesh struct containing (and owning) the mesh and associated textures
		// Returns nullptr if the model couldn't be loaded
		// Uses float for position, normal and texture coordinates
		// Uses unsigned int for indices
		// Only support 1 texture per mesh section (one model (mesh in chimp) can have multiple mesh sections!)
		virtual std::unique_ptr<ImportedMesh> LoadModel(const std::string& path, const Settings& settings = {}) = 0;

	protected:
		IRenderingManager& m_RenderingManager;
	};
}