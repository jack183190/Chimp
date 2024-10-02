#pragma once

#include "api/time/TimeManager.h"
#include "api/window/IWindow.h"

namespace Chimp {
	class EntryPoint;
	class Engine {
		friend class EntryPoint;
	private:
		Engine();

	public:
		[[nodiscard]] TimeManager& GetTimeManager();
		[[nodiscard]] IWindow& GetWindow();

	private:
		[[nodiscard]] std::unique_ptr<IWindow> CreateWindow() const;

	private:
		TimeManager m_TimeManager;
		std::unique_ptr<IWindow> m_Window;
	};
}