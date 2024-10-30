#include "MenuScene.h"

MenuScene::MenuScene(Chimp::Engine& engine)
	: m_Engine(engine),
	m_GameServer(std::make_shared<GameServer>(m_Engine)),
	m_GameClient(std::make_shared<GameClient>(m_Engine)),
	m_ConnectionInfo({})
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
	m_GameServer->Update();
	m_GameClient->Update();
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
	bool disableHostButton = m_GameServer->IsRunning();
	if (m_ConnectionInfo.HostName != "localhost") {
		ImGui::Text("You can only host on 'localhost' (you can connect on other ips)");
		disableHostButton = true;
	}
	ImGui::BeginDisabled(disableHostButton);
	if (ImGui::Button("Host Server")) {
		m_GameServer->Start(m_ConnectionInfo);
	}
	ImGui::EndDisabled();

	// Shutdown server
	ImGui::BeginDisabled(!m_GameServer->IsRunning());
	ImGui::SameLine();
	if (ImGui::Button("Shutdown Server")) {
		m_GameServer->Shutdown();
	}
	ImGui::EndDisabled();

	// Connect
	ImGui::BeginDisabled(m_GameClient->IsConnected());
	if (ImGui::Button("Connect")) {
		m_GameClient->Connect(m_ConnectionInfo);
	}
	ImGui::EndDisabled();

	// Disconnect
	ImGui::BeginDisabled(!m_GameClient->IsConnected());
	ImGui::SameLine();
	if (ImGui::Button("Disconnect")) {
		m_GameClient->Disconnect();
	}
	ImGui::EndDisabled();

	// Text
	if (m_GameServer->IsRunning()) {
		ImGui::Text("Server is being hosted");
	}
	else {
		ImGui::Text("Server is not being hosted");
	}
	if (m_GameClient->IsConnected()) {
		ImGui::Text("Client is connected");
	}
	else {
		ImGui::Text("Client is not connected");
	}

	ImGui::End();
}
