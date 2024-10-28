#pragma once

#include "api/time/TimeManager.h"
#include "api/window/IWindow.h"
#include "api/graphics/IRenderingManager.h"
#include "api/graphics/images/IImageLoader.h"
#include "api/assets/AssetManager.h"
#include "api/networking/EventHandler.h"
#include "api/networking/IServer.h"
#include "api/networking/IClient.h"
#include "api/logging/Logger.h"

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

		[[nodiscard]] std::unique_ptr<IServer> HostServer(const ConnectionInfo& serverInfo);
		[[nodiscard]] std::unique_ptr<IClient> ConnectToServer(const ConnectionInfo& serverInfo);

		// Create an enabled logger that logs to console and file
		// name - name of the logger, can be anything that is a valid directory name
		// level - minimum log level to log
		[[nodiscard]] std::unique_ptr<Logger> CreateLogger(const std::string& name, LogLevel level = LogLevel::INFO);

		// Create an event handler that can be used to send and receive events
		// EventType - enum representing event type
		// Event - struct representing event data, inheritance is supported
		template <typename EventType, typename Event>
		[[nodiscard]] EventHandlerAndBroadcaster<EventType, Event> CreateEventHandler() {
			return EventHandlerAndBroadcaster<EventType, Event>();
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