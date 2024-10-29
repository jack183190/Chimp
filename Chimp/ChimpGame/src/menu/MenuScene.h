#include "stdafx.h"
#include "GameRenderer.h"
#include "networking/GameNetworking.h"

class MenuScene : public Chimp::Scene {
public:
	MenuScene(Chimp::Engine& engine);
	~MenuScene();

public:
	bool ShouldExit() const override { return m_Engine.GetWindow().GetStatus() == Chimp::WindowStatus::SHOULD_CLOSE; }

protected:
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;

	void OnUpdate() override;
	void OnRender() override;

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	std::shared_ptr<GameNetworking> m_GameNetworking;
};