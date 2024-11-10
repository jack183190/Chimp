#pragma once

#include "api/assets/IModelImporter.h"

namespace Chimp {
	class ModelImporter : public Chimp::IModelImporter {
		friend class AssetManager;
	private:
		ModelImporter(IRenderingManager& renderingManager) :
			IModelImporter(renderingManager) {}
	public:
		virtual ~ModelImporter() = default;

		std::unique_ptr<ImportedMesh> LoadModel(const std::string& path, const Settings& settings = {}) override;

	private:
		Assimp::Importer m_Importer;
	};
}