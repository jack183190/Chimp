#include "MenuScene.h"
#include "game/GameScene.h"
#include "Debug.h"
#include "Logger.h"

MenuScene::MenuScene(Chimp::Engine& engine,
	std::shared_ptr<Chimp::GameShader> renderer)
	: m_Engine(engine),
	m_ConnectionInfo({})
{
	m_Engine.GetWindow().SetTitle("Chimp Challenge");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();

	if (renderer == nullptr) {
		// Our renderer
		m_GameShader = std::make_shared<Chimp::GameShader>(m_Engine);

#ifdef DEBUG_AUTOHOST_AUTOCONNECT
		Networking::GetServer()->Start(m_ConnectionInfo);
		Networking::GetClient()->Connect(m_ConnectionInfo);
#endif
	}
	else {
		m_GameShader = renderer;
		Networking::GetServer()->Shutdown();
		Networking::GetClient()->Disconnect();
	}
}

MenuScene::~MenuScene()
{

}

struct Test : public Chimp::YAMLSerialisable {
	int notSaved = 0;
	std::string name;
	std::unique_ptr<Test> child = nullptr;

	void Serialise(Chimp::YAMLBlock& block, const SerialiseChildFunc& serialiseChild) const override {
		block.Strings.insert({ "name", name });
		if (child != nullptr) {
			serialiseChild("child", *child);
		}
	}

	static std::unique_ptr<Test> Deserialise(const Chimp::YAMLBlock& block, const DeserialiseChildFunc& deserialiseChild) {
		auto test = std::make_unique<Test>();
		auto iter = block.Strings.find("name");
		if (iter != block.Strings.end()) {
			test->name = iter->second;
		}

		test->child = UNIQUE_PTR_CAST_AND_MOVE(Test, deserialiseChild("child"));
		return test;
	}
};

void MenuScene::OnInit()
{
	m_Engine.GetYAMLSerialiser().RegisterSerialisable<Test>("test", Test::Deserialise);

	Test test;
	test.notSaved = 1;
	test.name = "testobject";
	test.child = std::make_unique<Test>();
	test.child->notSaved = 2;
	test.child->name = "childobject";
	test.child->child = std::make_unique<Test>();
	test.child->child->name = "grandchildobject";

	m_Engine.GetYAMLSerialiser().WriteToFile(test, "test.yaml");

	std::unique_ptr<Test> testCopy = UNIQUE_PTR_CAST_AND_MOVE(Test, m_Engine.GetYAMLSerialiser().ReadFromFile("test.yaml"));
	if (testCopy != nullptr) {
		GetLogger().Info(std::format("Test notSaved: {}", testCopy->notSaved));
		GetLogger().Info(std::format("Test name: {}", testCopy->name));
		if (testCopy->child != nullptr) {
			GetLogger().Info(std::format("Child notSaved: {}", testCopy->child->notSaved));
			GetLogger().Info(std::format("Child name: {}", testCopy->child->name));
			if (testCopy->child->child != nullptr) {
				GetLogger().Info(std::format("Grandchild name: {}", testCopy->child->child->name));
			}
		}
	}
	else {
		GetLogger().Error("Failed to read test.yaml");
	}
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
		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameScene>(m_Engine, m_GameShader));
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
