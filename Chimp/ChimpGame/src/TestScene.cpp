#include "TestScene.h"

TestScene::TestScene(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetWindow().SetTitle("Test Scene");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	// shader
	Chimp::ShaderFilePaths shaderFilePaths = {};
	{
		shaderFilePaths.Vertex = GAME_SRC + std::string("/shaders/default.vert");
		shaderFilePaths.Fragment = GAME_SRC + std::string("/shaders/default.frag");
	}
	m_Shader = renderingManager.CompileShader(shaderFilePaths);

	if (!m_Shader->IsValid())
	{
		std::cerr << "Failed to compile shader" << std::endl;
		exit(-1);
	}
	std::cout << "Shader compiled successfully" << std::endl;

	// Camera
	std::shared_ptr<Chimp::IBuffer> m_CameraBuffer = renderingManager.CreateBuffer(
		Chimp::GraphicsType::FLOAT,
		sizeof(Chimp::Matrix),
		1,
		{
			Chimp::Usage::UpdateFrequency::OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);

	const auto& cameraMatrices = renderingManager.GetRenderer().GetDefaultCamera().GetCameraMatrices();
	Chimp::Matrix cameraMatrix = cameraMatrices.GetProjectionMatrix() * cameraMatrices.GetViewMatrix();
	m_CameraBufferId = m_Shader->GetShaderBuffers().AddBuffer({ m_CameraBuffer, "Camera" });

	// Controller
	m_CameraController = std::make_unique<Chimp::DebugCameraController>(
		renderingManager.GetRenderer().GetDefaultCamera(),
		m_Engine.GetWindow().GetInputManager()
	);

	// Model buffer
	m_ModelBuffer = renderingManager.CreateBuffer(
		{
			Chimp::Usage::UpdateFrequency::OFTEN,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
	const auto windowSize = m_Engine.GetWindow().GetSize();
	Chimp::Matrix modelMatrix = Chimp::CreateScaleMatrix({ windowSize.x / 2.0f, windowSize.y / 2.0f, 1.0f });
	modelMatrix *= Chimp::CreateTranslationMatrix({ -0.5f, 0.5f, 1.0f });
	Chimp::RawArray modelArray;
	modelArray.NumberElements = 1;
	modelArray.Size = sizeof(Chimp::Matrix);
	modelArray.Data = memcpy(new Chimp::Matrix[modelArray.NumberElements], &modelMatrix, modelArray.Size);
	m_ModelBuffer->SetData(Chimp::GraphicsType::FLOAT,
		modelArray);
	const auto modelId = m_Shader->GetShaderBuffers().AddBuffer({ m_ModelBuffer, "Model" });
	m_Shader->UpdateShaderBuffer(modelId);

	// Mesh
	m_Mesh = Chimp::TexturedQuad::Create(renderingManager, m_Shader);

	// Texture
	m_Texture = renderingManager.CreateTextureFromImage(
		GAME_SRC + std::string("/textures/tex.png")
	);
	assert(m_Texture != nullptr);
	m_Shader->SetTextureSampler("u_ActiveTexture", *m_Texture);
}

TestScene::~TestScene()
{
}

void TestScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
}

void TestScene::OnDeactivate()
{
}

void TestScene::OnUpdate()
{
	// Test input
	{
		if (m_Engine.GetWindow().GetInputManager().IsKeyDown(Chimp::Keyboard::SPACE))
		{
			std::cout << "Space key is pressed" << std::endl;
		}

		if (m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::B))
		{
			std::cout << "B key is pressed" << std::endl;
		}

		if (m_Engine.GetWindow().GetInputManager().IsMouseButtonDown(Chimp::Mouse::LEFT))
		{
			std::cout << "Left mouse button is down" << std::endl;
			auto pos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
			std::cout << "Mouse pos is " << pos.x << ", " << pos.y << std::endl;
		}

		if (m_Engine.GetWindow().GetInputManager().IsMouseButtonPressed(Chimp::Mouse::RIGHT))
		{
			std::cout << "Right mouse button is pressed" << std::endl;
		}
	}

	// Update camera
	m_CameraController->OnUpdate(m_Engine.GetTimeManager().GetDeltaTime());
	{
		const auto& cameraMatrices = m_Engine.GetRenderingManager().GetRenderer().GetDefaultCamera().GetCameraMatrices();
		const Chimp::Matrix cameraMatrix = cameraMatrices.GetProjectionMatrix() * cameraMatrices.GetViewMatrix();
		m_Shader->SetShaderBufferSubData(m_CameraBufferId, &cameraMatrix, sizeof(Chimp::Matrix));
	}
}

void TestScene::OnRender()
{
	m_Engine.GetRenderingManager().GetRenderer().Draw(*m_Mesh);
}
