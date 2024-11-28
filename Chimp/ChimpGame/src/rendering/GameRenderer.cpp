#include "GameRenderer.h"
#include "entities/EntityIdComponent.h"
#include "entities/health/HealthComponent.h"

using namespace Chimp;

GameRenderer::GameRenderer(
	Engine& engine) :
	m_Engine(engine),
	m_Camera(&engine.GetRenderingManager().GetRenderer().GetDefaultCamera())
{
	auto& renderingManager = m_Engine.GetRenderingManager();

	// COMPILE SHADER
	{
		// GAME_SRC defined by ChimpGame cmake
		m_ShaderFilePaths.Vertex = GAME_SRC + std::string("/assets/shaders/default.vert");
		m_ShaderFilePaths.Fragment = GAME_SRC + std::string("/assets/shaders/default.frag");
	}
	m_Shader = m_Engine.GetResourceManager().GetShaders().ImmediateDepend(m_ShaderFilePaths);

	// CAMERA BUFFER
	std::shared_ptr<Chimp::IBuffer> cameraBuffer = renderingManager.CreateBuffer(
		sizeof(Chimp::Matrix),
		1,
		{
			Chimp::Usage::UpdateFrequency::OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
	m_CameraBufferId = m_Shader->GetShaderBuffers().AddBuffer({ cameraBuffer, "Camera" });

	// MODEL BUFFER
	std::shared_ptr<Chimp::IBuffer> modelBuffer = renderingManager.CreateBuffer(
		sizeof(Chimp::Matrix),
		1,
		{
			Chimp::Usage::UpdateFrequency::OFTEN,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
	m_ModelBufferId = m_Shader->GetShaderBuffers().AddBuffer({ modelBuffer, "Model" });
}

GameRenderer::~GameRenderer()
{
	m_Engine.GetResourceManager().GetShaders().Release(m_ShaderFilePaths);
}

void GameRenderer::SetCamera(Camera* camera)
{
	if (camera == nullptr)
	{
		m_Camera = &m_Engine.GetRenderingManager().GetRenderer().GetDefaultCamera();
	}
	else
	{
		m_Camera = camera;
	}
}

void GameRenderer::BeginFrame()
{
	m_IsFrameBegun = true;
	// Update camera
	Chimp::Matrix cameraMatrix = m_Camera->GetCameraMatrices().GetProjectionMatrix() * m_Camera->GetCameraMatrices().GetViewMatrix();
	m_Shader->SetShaderBufferSubData(m_CameraBufferId, &cameraMatrix, sizeof(Chimp::Matrix), 0);
}

void GameRenderer::Render(const Chimp::Mesh& mesh, const Chimp::Matrix& transform)
{
	assert(m_IsFrameBegun);

	// Send transform
	m_Shader->SetShaderBufferSubData(m_ModelBufferId, &transform, sizeof(Chimp::Matrix), 0);

	for (const auto& section : mesh)
	{
		// Send the texture
		assert(section.Texture); // This shader doesn't support no texture
		m_Shader->SetTextureSampler(
			"u_ActiveTexture",
			section.Texture->GetResource()
		);

		// Draw the section
		m_Engine.GetRenderingManager().GetRenderer().Draw(section, *m_Shader);
	}
}

void GameRenderer::RenderWorld(Chimp::ECS& ecs)
{
	struct Renderable
	{
		float Z;
		Chimp::Matrix TransformMatrix;
		Chimp::Mesh* Mesh;
	};
	auto zSorter = [](
		const Renderable& a,
		const Renderable& b) {
			return a.Z > b.Z;
		};
	std::vector<Renderable> renderQueue;

	auto view = ecs.GetEntitiesWithComponents<TransformComponent, EntityIdComponent, MeshComponent>();
	for (auto& [transform, id, mesh] : view)
	{
		// if has health, dont render if dead
		auto health = ecs.GetComponent<HealthComponent>(id.Id);
		if (health.HasValue() && health->Health <= 0)
		{
			continue;
		}

		Renderable renderable;
		renderable.TransformMatrix = transform.GetTransformMatrix();
		assert(mesh.Mesh != nullptr);
		renderable.Mesh = mesh.Mesh;
		renderable.Z = transform.GetTranslation().z;
		renderQueue.push_back(renderable);
	}

	std::sort(renderQueue.begin(), renderQueue.end(), zSorter);

	for (const auto& renderable : renderQueue)
	{
		Render(*renderable.Mesh, renderable.TransformMatrix);
	}
}

std::string GameRenderer::LoadSprite(Chimp::Engine& engine, const std::string& name, const std::string& path)
{
	engine.GetResourceManager().GetMeshStorage().CreateTexturedQuad(name, std::string(GAME_SRC) + "/assets/textures/" + path);
	return name;
}

void GameRenderer::UnloadSprite(Chimp::Engine& engine, const std::string& name)
{
	engine.GetResourceManager().GetMeshStorage().DestroyStoredMesh(name);
}

void GameRenderer::UnloadSprites(Chimp::Engine& engine, const std::vector<std::string>& names)
{
	for (const auto& name : names)
	{
		engine.GetResourceManager().GetMeshStorage().DestroyStoredMesh(name);
	}
}
