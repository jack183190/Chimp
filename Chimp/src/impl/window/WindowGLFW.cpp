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
		// Keys pressed
		{
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

		// Mouse buttons pressed
		{
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
				auto* win = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
				if (action == GLFW_PRESS) {
					win->m_InputManager.OnMouseDown(static_cast<Mouse::Button>(button));
				}
				else if (action == GLFW_RELEASE) {
					win->m_InputManager.OnMouseUp(static_cast<Mouse::Button>(button));
				}
				});
		}

		// Mouse position
		{
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
				auto* win = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
				win->m_InputManager.SetMousePosition({ xpos, ypos });
				});
		}
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
		if (m_Status == WindowStatus::FAILED_INITIALIZATION) [[unlikely]] {
			return;
			}
		glfwSwapBuffers(m_Window);

	}

	Vector2f WindowGLFW::GetSize() const
	{
		Vector2i size{};
		glfwGetWindowSize(m_Window, &size.x, &size.y);
		return size;
	}

	void WindowGLFW::SetSize(const Vector2f& size)
	{
		glfwSetWindowSize(m_Window, static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void WindowGLFW::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	void WindowGLFW::SetResizable(bool resizable)
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable);
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