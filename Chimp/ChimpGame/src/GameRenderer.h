#pragma once

#include "Chimp.h"

using namespace Chimp;

class GameRenderer
{
public:
	GameRenderer(Chimp::Engine& engine, 
		std::shared_ptr<IShader> shader);
	
public:
	void SetCamera(Camera* camera);

	void BeginFrame();

	void Render(const Chimp::Mesh &mesh, const Chimp::Matrix &transform);

private:
	Engine& m_Engine;
	Chimp::IShaderBuffers::Index m_CameraBufferId;
	Camera* m_Camera;
	Chimp::IShaderBuffers::Index m_ModelBufferId;
	std::shared_ptr<IShader> m_Shader;
};