#include "api/window/InputManager.h"

namespace Chimp {
	InputManager::InputManager() :
		m_KeysDown(), m_KeysPressed()
	{
		m_KeysDown.fill(false);
		m_KeysPressed.fill(false);
	}

	void InputManager::OnKeyDown(Chimp::Keyboard::Key key)
	{
		if (!m_KeysDown[key]) {
			m_KeysPressed[key] = true;
		}

		m_KeysDown[key] = true;
	}

	void InputManager::OnKeyUp(Chimp::Keyboard::Key key)
	{
		m_KeysDown[key] = false;
	}

	void InputManager::UpdateEnd()
	{
		m_KeysPressed.fill(false);
	}

	bool InputManager::IsKeyDown(Chimp::Keyboard::Key key) const
	{
		return m_KeysDown[key];
	}

	bool InputManager::IsKeyPressed(Chimp::Keyboard::Key key) const
	{
		return m_KeysPressed[key] ;
	}
}