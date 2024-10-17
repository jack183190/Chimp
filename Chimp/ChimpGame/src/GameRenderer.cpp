#include "GameRenderer.h"

GameRenderer::GameRenderer(
	Chimp::Engine& engine,
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
	// Update camera
	Chimp::Matrix cameraMatrix = m_Camera->GetCameraMatrices().GetProjectionMatrix() * m_Camera->GetCameraMatrices().GetViewMatrix();
	m_Shader->SetShaderBufferSubData(m_CameraBufferId, &cameraMatrix, sizeof(Chimp::Matrix), 0);
}

void GameRenderer::Render(const Chimp::Mesh& mesh, const Chimp::Matrix& transform)
{
	// Send transform
	m_Shader->SetShaderBufferSubData(m_ModelBufferId, &transform, sizeof(Chimp::Matrix), 0);

	// Render
	m_Engine.GetRenderingManager().GetRenderer().Draw(mesh);
}
