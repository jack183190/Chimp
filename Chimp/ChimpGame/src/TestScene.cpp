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
	auto shader = m_Engine.GetAssetManager().LoadShader(shaderFilePaths);

	// Mesh
	auto& texture = m_Engine.GetAssetManager().LoadTexture(GAME_SRC + std::string("/textures/tex.png"));
	m_Mesh = Chimp::TexturedQuad::Create(renderingManager, texture);

	// Our renderer
	m_GameRenderer = std::make_unique<GameRenderer>(m_Engine, shader);

	// ECS
	{
		struct a { int da; };
		struct b { int db; };
		struct c { int dc; };
		struct d { int dd; };
		struct e { int de; };

		// testing adding setting comps
		{
			auto ent = m_ECS.CreateEntity();
			m_ECS.SetComponent(ent, a{ 1 });
			std::cout << "a: " << m_ECS.GetComponent<a>(ent)->da << std::endl;
			m_ECS.GetMutableComponent<a>(ent)->da = 2;
			std::cout << "a: " << m_ECS.GetComponent<a>(ent)->da << std::endl;
			std::cout << "b ptr: " << m_ECS.GetComponent<b>(ent) << std::endl;
		}
		// timing get entities
		{
			for (int i = 0; i < 1000; ++i) {
				auto ent = m_ECS.CreateEntity();
				m_ECS.SetComponent(ent, a{ i });
				m_ECS.SetComponent(ent, b{ i + 1 });
				m_ECS.SetComponent(ent, c{ i + 2 }); // not in system
				m_ECS.SetComponent(ent, d{ i + 3 });
				if (rand() % 2 == 0)	m_ECS.SetComponent(ent, e{ i + 4 });
			}

			int num = 0;
			auto now = std::chrono::high_resolution_clock::now();
			auto view = m_ECS.GetEntitiesWithComponents<a, b, d, e>();
			auto end = std::chrono::high_resolution_clock::now();
			for (auto& [compA, compB, compD, compE] : view) {
				num++;
			}

			std::cout << "Time to get entities: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - now).count() << "ns" << std::endl;
			std::cout << "Num entities: " << num << std::endl;
		}
	}
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
