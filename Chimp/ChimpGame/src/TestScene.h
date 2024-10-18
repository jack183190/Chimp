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

private:
	Engine& m_Engine;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<GameRenderer> m_GameRenderer;
};