#pragma once

#include "Chimp.h"

// Simple abstraction around Chimp's renderer which is specific to the shader (so it sends correct textures, uniforms, etc)
class GameRenderer
{
public:
	GameRenderer(Chimp::Engine& engine);
	
public:
	void SetCamera(Chimp::Camera* camera);

	void BeginFrame();

	void Render(const Chimp::Mesh &mesh, const Chimp::Matrix &transform);

	// Render world, where entities with maximum z are rendered first
	void RenderWorld(Chimp::ECS& ecs);

	// Create and load textured quad into asset manager
	// engine - engine instance
	// name - name of the asset
	// path - path to the texture (relative to assets/textures)
	// returns name parameter
	static std::string LoadSprite(Chimp::Engine& engine, const std::string& name, const std::string& path);
	static void UnloadSprite(Chimp::Engine& engine, const std::string& name);
	static void UnloadSprites(Chimp::Engine& engine, const std::vector<std::string>& names);

private:
	Chimp::Engine& m_Engine;
	Chimp::ShaderBufferId m_CameraBufferId;
	Chimp::Camera* m_Camera;
	Chimp::ShaderBufferId m_ModelBufferId;
	std::shared_ptr<Chimp::IShader> m_Shader;
	bool m_IsFrameBegun = false;
};