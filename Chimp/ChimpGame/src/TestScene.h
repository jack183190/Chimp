#include "Chimp.h"

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
	Chimp::Engine& m_Engine;
	std::unique_ptr<Chimp::Mesh> m_Mesh;
	Chimp::IShaderBuffers::Index m_CameraBufferId;
	std::shared_ptr<Chimp::IBuffer> m_ModelBuffer;
	std::unique_ptr<Chimp::ITexture> m_Texture;
	std::shared_ptr<Chimp::IShader> m_Shader;
	std::unique_ptr<Chimp::DebugCameraController> m_CameraController;
};