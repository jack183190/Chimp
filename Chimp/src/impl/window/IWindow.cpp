#include "api/window/IWindow.h"

namespace Chimp {
	WindowStatus IWindow::GetStatus() const
	{
		return m_Status;
	}

	InputManager& IWindow::GetInputManager()
	{
		return m_InputManager;
	}
}