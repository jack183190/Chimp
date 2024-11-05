#include "GameRenderer.h"

using namespace Chimp;

GameRenderer::GameRenderer(
	Engine& engine,
	std::shared_ptr<IShader> shader) :
	m_Engine(engine),
	m_Camera(&engine.GetRenderingManager().GetRenderer().GetDefaultCamera()),
	m_Shader(shader)
{
	auto& renderingManager = m_Engine.GetRenderingManager();

	// CAMERA BUFFER
	std::shared_ptr<Chimp::IBuffer> m_CameraBuffer = renderingManager.CreateBuffer(
		sizeof(Chimp::Matrix),
		1,
		{
			Chimp::Usage::UpdateFrequency::OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
	m_CameraBufferId = m_Shader->GetShaderBuffers().AddBuffer({ m_CameraBuffer, "Camera" });

	// MODEL BUFFER
	std::shared_ptr<Chimp::IBuffer> m_ModelBuffer = renderingManager.CreateBuffer(
		sizeof(Chimp::Matrix),
		1,
		{
			Chimp::Usage::UpdateFrequency::OFTEN,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
	m_ModelBufferId = m_Shader->GetShaderBuffers().AddBuffer({ m_ModelBuffer, "Model" });
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
		assert(section.Texture != nullptr);
		m_Shader->SetTextureSampler(
			"u_ActiveTexture",
			*section.Texture
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
			return a.Z < b.Z;
		};
	std::vector<Renderable> renderQueue;

	auto view = ecs.GetEntitiesWithComponents<TransformComponent, MeshComponent>();
	for (auto& [transform, mesh] : view)
	{
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
