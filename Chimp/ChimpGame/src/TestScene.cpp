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
			for (auto& [compA, compB, compD, compE] : view) {
				num++;
			}
			auto end = std::chrono::high_resolution_clock::now();

			std::cout << "Time to get entities: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - now).count() << "ns" << std::endl;
			std::cout << "Num entities: " << num << std::endl;
		}
	}

	// Event handler
	{
		enum class EventType {
			ONE, TWO
		};
		class Event {};
		class OneEvent : public Event {
		public:
			int a;
		};
		class TwoEvent : public Event {
		public:
			int b;
		};
		auto pair = m_Engine.CreateEventHandler<EventType, Event>();
		OneEvent eventOne;
		eventOne.a = 1;
		pair.Broadcaster->Broadcast(EventType::ONE, eventOne);
		auto listenerId = pair.Handler->Subscribe(EventType::ONE, [](const Event* event) {
			auto oneEvent = static_cast<const OneEvent*>(event);
			std::cout << "One event: " << oneEvent->a << std::endl;
			});
		pair.Handler->Subscribe(EventType::TWO, [](const Event* event) {
			auto twoEvent = static_cast<const TwoEvent*>(event);
			std::cout << "Two event: " << twoEvent->b << std::endl;
			});
		pair.Broadcaster->Broadcast(EventType::ONE, eventOne);
		pair.Handler->Unsubscribe(listenerId);
		pair.Broadcaster->Broadcast(EventType::ONE, eventOne);
		TwoEvent eventTwo;
		eventTwo.b = 2;
		pair.Broadcaster->Broadcast(EventType::TWO, eventTwo);
	}

	// Networking
	{
		// Server
		{
			Chimp::ServerInfo serverInfo;
			serverInfo.IsHost = true;
			serverInfo.HostName = "localhost";
			serverInfo.Port = 37478;
			serverInfo.MaxClients = 32;
			serverInfo.MaxChannels = 2;
			serverInfo.MaxIncomingBandwidth = 0;
			serverInfo.MaxOutgoingBandwidth = 0;
			m_Server = m_Engine.ConnectOrHostServer(serverInfo);
		}

		// Client 1
		std::thread t1([this]() {
			Chimp::ServerInfo serverInfo;
			serverInfo.IsHost = false;
			serverInfo.HostName = "localhost";
			serverInfo.Port = 37478;
			serverInfo.MaxClients = 1;
			serverInfo.MaxChannels = 2;
			serverInfo.MaxIncomingBandwidth = 0;
			serverInfo.MaxOutgoingBandwidth = 0;
			serverInfo.ConnectionTimeoutMs = 500;
			m_Client1 = m_Engine.ConnectOrHostServer(serverInfo);
			});
		t1.detach();
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
	// Networking
	{
		m_Server->Update();
	}
}

void TestScene::OnRender()
{
	m_GameRenderer->BeginFrame();

	m_GameRenderer->Render(*m_Mesh, Chimp::CreateIdentityMatrix());
}
