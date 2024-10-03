#pragma once

#include "stdafx.h"
#include "InputManager.h"

namespace Chimp {
	enum class WindowStatus {
		 SHOULD_CLOSE, OPEN, FAILED_INITIALIZATION
	};

	class MainLoop;
	class IWindow {
		friend class MainLoop;
	protected:
		IWindow() = default;

	public:
		~IWindow() = default;

	protected:
		// Will be called at the beginning of every frame.
		// This function should poll any events and update m_ShouldClose
		virtual void Update() = 0;

		// Will be called at the end of every frame.
		void UpdateEnd();

		// Will be called after rendering of the frame is complete.
		// This function should swap the front and back buffers.
		virtual void SwapBuffers() = 0;

	public:
		// Returns the current state of the window
		[[nodiscard]] WindowStatus GetStatus() const;

		// Returns the InputManager for this window.
		[[nodiscard]] InputManager& GetInputManager();

	protected:
		WindowStatus m_Status = WindowStatus::FAILED_INITIALIZATION;
		InputManager m_InputManager;
	};
}