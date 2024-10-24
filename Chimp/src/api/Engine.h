#pragma once

#include "api/time/TimeManager.h"
#include "api/window/IWindow.h"
#include "api/graphics/IRenderingManager.h"
#include "api/graphics/images/IImageLoader.h"
#include "api/assets/AssetManager.h"
#include "api/networking/EventHandler.h"
#include "api/networking/IServer.h"

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
		[[nodiscard]] AssetManager& GetAssetManager();

		[[nodiscard]] std::unique_ptr<IServer> ConnectOrHostServer(const ServerInfo& serverInfo);

		// Create an event handler that can be used to send and receive events
		// EventType - enum representing event type
		// Event - struct representing event data, inheritance is supported
		template <typename EventType, typename Event>
		[[nodiscard]] EventHandlerAndBroadcaster<EventType, Event> CreateEventHandler() {
			EventHandlerAndBroadcaster<EventType, Event> pair;
			pair.Handler = std::make_unique<EventHandler<EventType, Event>>(pair.Broadcaster);
			return pair;
		}

	private:
		[[nodiscard]] std::unique_ptr<IWindow> CreateWindow() const;
		[[nodiscard]] std::unique_ptr<IRenderingManager> CreateRenderingManager() const;
		[[nodiscard]] std::unique_ptr<IImageLoader> CreateImageLoader() const;

	private:
		AssetManager m_AssetManager;
		TimeManager m_TimeManager;
		std::unique_ptr<IWindow> m_Window;
		std::unique_ptr<IImageLoader> m_ImageLoader; // must be above rendering manager
		std::unique_ptr<IRenderingManager> m_RenderingManager;
	};
}