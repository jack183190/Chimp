#include "MenuScene.h"
#include "game/GameScene.h"

MenuScene::MenuScene(Chimp::Engine& engine,
	std::shared_ptr<GameRenderer> renderer)
	: m_Engine(engine),
	m_ConnectionInfo({})
{
	m_Engine.GetWindow().SetTitle("Chimp Challenge");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	if (renderer == nullptr) {
		// Shader
		Chimp::ShaderFilePaths shaderFilePaths = {};
		{
			shaderFilePaths.Vertex = GAME_SRC + std::string("/shaders/default.vert");
			shaderFilePaths.Fragment = GAME_SRC + std::string("/shaders/default.frag");
		}
		auto shader = m_Engine.GetAssetManager().LoadShader(shaderFilePaths);

		// Our renderer
		m_GameRenderer = std::make_shared<GameRenderer>(m_Engine, shader);

		Networking::GetServer()->Start(m_ConnectionInfo);
		Networking::GetClient()->Connect(m_ConnectionInfo);
	}
	else {
		m_GameRenderer = renderer;
		Networking::GetServer()->Shutdown();
		Networking::GetClient()->Disconnect();
	}
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
	if (!Networking::GetClient()->IsConnected()) return;
	auto& clientHandlers = Networking::GetClient()->GetHandlers();
	
	if (clientHandlers.CurrentMatchHandler->IsInMatch()) {
		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameScene>(m_Engine, m_GameRenderer));
	}
	
}

void MenuScene::OnRender()
{

}

void MenuScene::OnRenderUI()
{
	ImGui::Begin("Networking", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	// Host
	static char buffer[256] = "";
	strcpy(buffer, m_ConnectionInfo.HostName.c_str());
	if (ImGui::InputText("Host", buffer, 256)) {
		m_ConnectionInfo.HostName = buffer;
	}

	// Port
	int port = m_ConnectionInfo.Port;
	ImGui::InputInt("Port", &port);
	if (port > 0 && port < 65535) {
		m_ConnectionInfo.Port = port;
	}

	// Timeout
	int timeout = m_ConnectionInfo.ConnectionTimeoutMs;
	ImGui::InputInt("Timeout", &timeout);
	if (timeout > 0) {
		m_ConnectionInfo.ConnectionTimeoutMs = timeout;
	}

	// Host
	bool disableHostButton = Networking::GetServer()->IsRunning();
	if (m_ConnectionInfo.HostName != "localhost") {
		ImGui::Text("You can only host on 'localhost' (you can connect on other ips)");
		disableHostButton = true;
	}
	ImGui::BeginDisabled(disableHostButton);
	if (ImGui::Button("Host Server")) {
		Networking::GetServer()->Start(m_ConnectionInfo);
	}
	ImGui::EndDisabled();

	// Shutdown server
	ImGui::BeginDisabled(!Networking::GetServer()->IsRunning());
	ImGui::SameLine();
	if (ImGui::Button("Shutdown Server")) {
		Networking::GetServer()->Shutdown();
	}
	ImGui::EndDisabled();

	// Connect
	ImGui::BeginDisabled(Networking::GetClient()->IsConnected());
	if (ImGui::Button("Connect")) {
		Networking::GetClient()->Connect(m_ConnectionInfo);
	}
	ImGui::EndDisabled();

	// Disconnect
	ImGui::BeginDisabled(!Networking::GetClient()->IsConnected());
	ImGui::SameLine();
	if (ImGui::Button("Disconnect")) {
		Networking::GetClient()->Disconnect();
	}
	ImGui::EndDisabled();

	// Text
	if (Networking::GetServer()->IsRunning()) {
		ImGui::Text("Server is being hosted");
	}
	else {
		ImGui::Text("Server is not being hosted");
	}
	if (Networking::GetClient()->IsConnected()) {
		auto str = "Client is connected with id " + std::to_string(Networking::GetClient()->GetClient().GetId());
		ImGui::Text(str.c_str());
	}
	else {
		ImGui::Text("Client is not connected");
	}

	ImGui::End();
}
