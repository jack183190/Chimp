#pragma once

#include "api/time/TimeManager.h"
#include "api/window/IWindow.h"
#include "api/graphics/IRenderingManager.h"
#include "api/graphics/images/IImageLoader.h"

namespace Chimp {
	class EntryPoint;
	class Engine {
		friend class EntryPoint;
	private:
		Engine();

	public:
		[[nodiscard]] TimeManager& GetTimeManager();
		[[nodiscard]] IWindow& GetWindow();
		[[nodiscard]] IRenderingManager& GetRenderingManager();

	private:
		[[nodiscard]] std::unique_ptr<IWindow> CreateWindow() const;
		[[nodiscard]] std::unique_ptr<IRenderingManager> CreateRenderingManager() const;
		[[nodiscard]] std::unique_ptr<IImageLoader> CreateImageLoader() const;

	private:
		TimeManager m_TimeManager;
		std::unique_ptr<IWindow> m_Window;
		std::unique_ptr<IImageLoader> m_ImageLoader; // must be above rendering manager
		std::unique_ptr<IRenderingManager> m_RenderingManager;
	};
}