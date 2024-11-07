#include "stdafx.h"
#include "rendering/GameRenderer.h"
#include "networking/Networking.h"

class MenuScene : public Chimp::Scene {
public:
	MenuScene(Chimp::Engine& engine, std::shared_ptr<GameRenderer> renderer = nullptr);
	~MenuScene();

public:

protected:
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;

	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<GameRenderer> m_GameRenderer;
	Chimp::ConnectionInfo m_ConnectionInfo;
};