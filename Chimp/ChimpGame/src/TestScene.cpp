#include "TestScene.h"

TestScene::TestScene(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetWindow().SetTitle("Test Scene");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	// vertex buffer
	const Chimp::GraphicsType VERTEX_TYPE = Chimp::GraphicsType::FLOAT;
	Chimp::RawArray vertexData;
	vertexData.NumberElements = 9;
	vertexData.Size = VERTEX_TYPE.Size * vertexData.NumberElements;
	vertexData.Data = new float[vertexData.NumberElements] {
		-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

	std::shared_ptr<Chimp::IBuffer> vertexBuffer = renderingManager.CreateBuffer(
		{
			Chimp::Usage::UpdateFrequency::VERY_OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::VERTEX_BUFFER
	);
	vertexBuffer->SetData(VERTEX_TYPE, vertexData);

	// index buffer
	const Chimp::GraphicsType INDEX_TYPE = Chimp::GraphicsType::UNSIGNED_INT;
	Chimp::RawArray indexData;
	indexData.NumberElements = 3;
	indexData.Size = INDEX_TYPE.Size * indexData.NumberElements;
	indexData.Data = new uint32_t[indexData.NumberElements]{
		0, 1, 2
	};

	auto indexBuffer = renderingManager.CreateBuffer(
		{
			Chimp::Usage::UpdateFrequency::VERY_OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::INDEX_BUFFER
	);
	indexBuffer->SetData(INDEX_TYPE, indexData);

	// element array
	std::shared_ptr<Chimp::IElementArrayLayout> elementLayout = renderingManager.CreateElementArrayLayout(
		Chimp::PrimitiveType::TRIANGLES,
		{
			{ Chimp::GraphicsType::FLOAT, 3, false }
		}
	);

	auto m_ElementArray = renderingManager.CreateElementArray(
		vertexBuffer,
		std::move(indexBuffer),
		elementLayout
	);

	// shader
	Chimp::ShaderFilePaths shaderFilePaths = {};
	{
		shaderFilePaths.Vertex = "C:/Coding/Uni/CMP316/Chimp/Chimp/ChimpGame/shaders/default.vert";
		shaderFilePaths.Fragment = "C:/Coding/Uni/CMP316/Chimp/Chimp/ChimpGame/shaders/default.frag";
	}
	auto m_Shader = renderingManager.CompileShader(shaderFilePaths);

	if (!m_Shader->IsValid())
	{
		std::cerr << "Failed to compile shader" << std::endl;
		exit(-1);
	}
	std::cout << "Shader compiled successfully" << std::endl;

	// Camera
	m_CameraBuffer = renderingManager.CreateBuffer(
		{
			Chimp::Usage::UpdateFrequency::OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
	Chimp::Matrix cameraMatrix = Chimp::CreateTranslationMatrix({ 0.0f, 1.0f, 0.0f });
	Chimp::RawArray cameraArray;
	cameraArray.NumberElements = 1;
	cameraArray.Size = sizeof(Chimp::Matrix);
	cameraArray.Data = memcpy(new Chimp::Matrix[cameraArray.NumberElements], &cameraMatrix, cameraArray.Size);
	m_CameraBuffer->SetData(Chimp::GraphicsType::FLOAT,
		cameraArray);
	const auto cameraId = m_Shader->GetShaderBuffers().AddBuffer({ m_CameraBuffer, "Camera" });
	m_Shader->UpdateShaderBuffer(cameraId);

	// Mesh
	m_Mesh = Chimp::Mesh::Builder()
		.AddSection(
			std::move(m_ElementArray),
			std::move(m_Shader)
		)
		.Build();
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
	if (m_Engine.GetWindow().GetInputManager().IsKeyDown(Chimp::Keyboard::SPACE))
	{
		std::cout << "Space key is pressed" << std::endl;
	}

	if (m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::A))
	{
		std::cout << "A key is pressed" << std::endl;
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

void TestScene::OnRender()
{
	m_Engine.GetRenderingManager().GetRenderer().Draw(*m_Mesh);
}
