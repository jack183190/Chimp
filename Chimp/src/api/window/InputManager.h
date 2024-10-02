#pragma once

#include "stdafx.h"
#include "Keys.h"

namespace Chimp {
	class WindowGLFW;
	class InputManager {
		friend class IWindow;
		friend class WindowGLFW;
	private:
		InputManager();

		void OnKeyDown(Chimp::Keyboard::Key key);
		void OnKeyUp(Chimp::Keyboard::Key key);
	public:
		[[nodiscard]] bool IsKeyDown(Chimp::Keyboard::Key key) const;

	private:
		std::array<bool, Chimp::Keyboard::LAST> m_Keys;
	};
}