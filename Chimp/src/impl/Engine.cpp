#pragma once

#include "api/Engine.h"
#ifdef CHIMP_GLFW
#include "impl/window/WindowGLFW.h"
#endif

namespace Chimp {
	Engine::Engine() :
		m_Window(CreateWindow())
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

	std::unique_ptr<IWindow> Engine::CreateWindow() const
	{
#ifdef CHIMP_GLFW
		return std::make_unique<WindowGLFW>();
#endif
		std::cerr << "No window implementation available." << std::endl;
		return nullptr;
	}
}