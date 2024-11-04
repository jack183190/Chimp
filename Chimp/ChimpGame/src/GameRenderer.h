#pragma once

#include "Chimp.h"

class GameRenderer
{
public:
	GameRenderer(Chimp::Engine& engine, 
		std::shared_ptr<Chimp::IShader> shader);
	
public:
	void SetCamera(Chimp::Camera* camera);

	void BeginFrame();

	void Render(const Chimp::Mesh &mesh, const Chimp::Matrix &transform);

private:
	Chimp::Engine& m_Engine;
	Chimp::ShaderBufferId m_CameraBufferId;
	Chimp::Camera* m_Camera;
	Chimp::ShaderBufferId m_ModelBufferId;
	std::shared_ptr<Chimp::IShader> m_Shader;
};