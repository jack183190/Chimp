#include "TestScene.h"

TestScene::TestScene(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetWindow().SetTitle("Test Scene");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	// vertex buffer
	Chimp::RawArray vertexData;
	vertexData.NumberElements = 9;
	vertexData.Size = sizeof(float) * vertexData.NumberElements;
	vertexData.Data = new float[vertexData.NumberElements] {
		-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

	auto vertexBuffer = renderingManager.CreateBuffer(
		{
			Chimp::Usage::UpdateFrequency::VERY_OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::VERTEX_BUFFER
	);
	vertexBuffer->SetData(vertexData);

	// index buffer
	Chimp::RawArray indexData;
	indexData.NumberElements = 3;
	indexData.Size = sizeof(uint32_t) * indexData.NumberElements;
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
	indexBuffer->SetData(indexData);

	// element array
	m_ElementArray = renderingManager.CreateElementArray(
		std::move(vertexBuffer),
		std::move(indexBuffer),
		renderingManager.CreateElementArrayLayout(
			{
				{ Chimp::GraphicsType::FLOAT, 3, false }
			}
		)
		);

	// shader
	Chimp::ShaderFilePaths shaderFilePaths = {};
	{
		shaderFilePaths.Vertex = "C:/Coding/Uni/CMP316/Chimp/Chimp/ChimpGame/shaders/default.vert";
		shaderFilePaths.Fragment = "C:/Coding/Uni/CMP316/Chimp/Chimp/ChimpGame/shaders/default.frag";
	}
	m_Shader = renderingManager.CompileShader(shaderFilePaths);

	if (!m_Shader->IsValid())
	{
		std::cerr << "Failed to compile shader" << std::endl;
		exit(-1);
	}
	std::cout << "Shader compiled successfully" << std::endl;
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
	m_Shader->Bind();
	m_ElementArray->Bind();
}
