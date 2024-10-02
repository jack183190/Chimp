#include "api/window/InputManager.h"

namespace Chimp {
	InputManager::InputManager() :
		m_Keys()
	{
		m_Keys.fill(false);
	}

	void InputManager::OnKeyDown(Chimp::Keyboard::Key key)
	{
		m_Keys[key] = true;
	}

	void InputManager::OnKeyUp(Chimp::Keyboard::Key key)
	{
		m_Keys[key] = false;
	}

	bool InputManager::IsKeyDown(Chimp::Keyboard::Key key) const
	{
		return m_Keys[key];
	}
}