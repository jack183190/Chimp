#pragma once

#include "api/Engine.h"
#ifdef CHIMP_GLFW
#include "impl/window/WindowGLFW.h"
#endif

#ifdef CHIMP_OPENGL
#include "impl/graphics/opengl/RenderingManager.h"
#endif

#ifdef CHIMP_STB
#include "impl/graphics/images/stb/ImageLoader.h"
#endif

namespace Chimp {
	Engine::Engine() :
		m_Window(CreateWindow()),
		m_ImageLoader(CreateImageLoader()),
		m_RenderingManager(CreateRenderingManager())
	{
	}

	TimeManager& Engine::GetTimeManager()
	{
		return m_TimeManager;
	}

	IWindow& Engine::GetWindow()
	{
		return *m_Window;
	}

	IRenderingManager& Engine::GetRenderingManager()
	{
		return *m_RenderingManager;
	}

	std::unique_ptr<IWindow> Engine::CreateWindow() const
	{
#ifdef CHIMP_GLFW
		return std::make_unique<WindowGLFW>();
#endif
		std::cerr << "No window implementation available." << std::endl;
		return nullptr;
	}

	std::unique_ptr<IRenderingManager> Engine::CreateRenderingManager() const
	{
		assert(m_ImageLoader);
#ifdef CHIMP_OPENGL
		return std::make_unique<GL::RenderingManager>(*m_ImageLoader);
#endif
		std::cerr << "No rendering manager implementation available." << std::endl;
		return nullptr;
	}

	std::unique_ptr<IImageLoader> Engine::CreateImageLoader() const
	{
#ifdef CHIMP_STB
		return std::make_unique<STB::ImageLoader>();
#endif
		std::cerr << "No image loader implementation available." << std::endl;
		return nullptr;
	}
}