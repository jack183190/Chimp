#include "api/window/IWindow.h"

namespace Chimp {
	void IWindow::UpdateEnd()
	{
		m_InputManager.UpdateEnd();
	}

	WindowStatus IWindow::GetStatus() const
	{
		return m_Status;
	}

	InputManager& IWindow::GetInputManager()
	{
		return m_InputManager;
	}
}