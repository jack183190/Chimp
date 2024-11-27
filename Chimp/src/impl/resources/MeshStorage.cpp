#include "api/resources/MeshStorage.h"
#include "api/Engine.h"
#include "Loggers.h"
#include "api/graphics/meshes/TexturedQuad.h"

namespace Chimp {
	MeshStorage::MeshStorage(Engine& engine)
		: m_Engine(engine)
	{
	}

	void MeshStorage::StoreMesh(const std::string& id, std::unique_ptr<Mesh> mesh)
	{
		if (m_Meshes.find(id) != m_Meshes.end())
		{
			Loggers::Resources().Error("Mesh already exists: " + id);
			return;
		}
		m_Meshes[id] = std::move(mesh);
	}

	void MeshStorage::CreateTexturedQuad(const std::string& id, const std::string& texturePath)
	{
		CreateTexturedQuad(id, m_Engine.GetResourceManager().LoadTexture(texturePath));
	}

	void MeshStorage::CreateTexturedQuad(const std::string& id, ITexture& texture)
	{
		auto mesh = TexturedQuad::Create(
			m_Engine.GetRenderingManager(),
			texture
		);
		StoreMesh(id, std::move(mesh));
	}

	Mesh& MeshStorage::GetMesh(const std::string& id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end());
		return *it->second;
	}

	bool MeshStorage::HasMesh(const std::string& id) const
	{
		return m_Meshes.find(id) != m_Meshes.end();
	}

	std::unique_ptr<Mesh> MeshStorage::ReclaimStoredMesh(const std::string& id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end());
		std::unique_ptr<Mesh> mesh = std::move(it->second);
		m_Meshes.erase(it);
		return std::move(mesh);
	}
}