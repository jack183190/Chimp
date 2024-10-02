#pragma once

#include "api/window/IWindow.h"

namespace Chimp {
	class WindowGLFW : public IWindow {
	public:
		WindowGLFW();
		~WindowGLFW();

	protected:
		void Update() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_Window = nullptr;

	private:
		static bool GLFWInitialized;
		static void InitGLFW();
	};
}