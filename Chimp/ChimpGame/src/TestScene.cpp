#include "TestScene.h"

TestScene::TestScene(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetWindow().SetTitle("Test Scene");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	// Shader
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

	// Mesh
	m_Mesh = Chimp::TexturedQuad::Create(renderingManager, m_Shader);

	// Texture
	m_Texture = renderingManager.CreateTextureFromImage(
		GAME_SRC + std::string("/textures/tex.png")
	);
	assert(m_Texture != nullptr);
	m_Shader->SetTextureSampler("u_ActiveTexture", *m_Texture);

	// Our renderer
	m_GameRenderer = std::make_unique<GameRenderer>(m_Engine, m_Shader);
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

}

void TestScene::OnRender()
{
	m_GameRenderer->BeginFrame();

	m_GameRenderer->Render(*m_Mesh, Chimp::CreateIdentityMatrix());
}
