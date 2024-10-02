#include "WindowGLFW.h"

namespace Chimp {
	WindowGLFW::WindowGLFW()
	{
		glfwSetErrorCallback([](int error, const char* description)
			{
				std::cerr << "GLFW Error " << error << ": " << description << std::endl;
			});

		InitGLFW();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
		if (!m_Window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			return;
		}

		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);

		m_Status = WindowStatus::OPEN;

		// Callbacks
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* win = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS) {
				win->m_InputManager.OnKeyDown(static_cast<Keyboard::Key>(key));
			}
			else if (action == GLFW_RELEASE) {
				win->m_InputManager.OnKeyUp(static_cast<Keyboard::Key>(key));
			}
			});
	}

	WindowGLFW::~WindowGLFW()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
		}
		glfwTerminate();
	}

	void WindowGLFW::Update()
	{
		if (m_Status == WindowStatus::FAILED_INITIALIZATION) [[unlikely]] {
			return;
			}
		else if (glfwWindowShouldClose(m_Window)) {
			m_Status = WindowStatus::SHOULD_CLOSE;
		}

		glfwPollEvents();
	}

	void WindowGLFW::SwapBuffers()
	{
	}

	bool WindowGLFW::GLFWInitialized = false;
	void WindowGLFW::InitGLFW()
	{
		if (GLFWInitialized) return;
		GLFWInitialized = true;

		if (!glfwInit()) {
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return;
		}
	}
}