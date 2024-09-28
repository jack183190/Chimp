#include "Chimp.h"

class TestScene : public Chimp::Scene {
public:
	TestScene(Chimp::Engine &engine);
	~TestScene();

public:
	bool ShouldExit() const override { return false; }

protected:
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;

	void OnUpdate() override;
	void OnRender() override;

private:
	Chimp::Engine& m_Engine;
};