#include "MenuScene.h"

MenuScene::MenuScene(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetWindow().SetTitle("Chimp Challenge");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	// Shader
	Chimp::ShaderFilePaths shaderFilePaths = {};
	{
		shaderFilePaths.Vertex = GAME_SRC + std::string("/shaders/default.vert");
		shaderFilePaths.Fragment = GAME_SRC + std::string("/shaders/default.frag");
	}
	auto shader = m_Engine.GetAssetManager().LoadShader(shaderFilePaths);

	// Our renderer
	m_GameRenderer = std::make_shared<GameRenderer>(m_Engine, shader);
}

MenuScene::~MenuScene()
{

}

void MenuScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
}

void MenuScene::OnDeactivate()
{
}

void MenuScene::OnUpdate()
{
}

void MenuScene::OnRender()
{

}
