#include "Chimp.h"
#include "GameRenderer.h"

class TestScene : public Chimp::Scene {
public:
	TestScene(Chimp::Engine &engine);
	~TestScene();

public:
	bool ShouldExit() const override { return m_Engine.GetWindow().GetStatus() == Chimp::WindowStatus::SHOULD_CLOSE; }

protected:
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;

	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

private:
	Engine& m_Engine;
	Logger m_Logger;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<GameRenderer> m_GameRenderer;
	ECS m_ECS;
	std::unique_ptr<IServer> m_Server;
	std::unique_ptr<IClient> m_Client1;
	std::unique_ptr<IClient> m_Client2;
};