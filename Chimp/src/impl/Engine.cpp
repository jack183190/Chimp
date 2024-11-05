#pragma once

#include "api/Engine.h"
#ifdef CHIMP_GLFW
#include "impl/window/WindowGLFW.h"
#endif

#ifdef CHIMP_OPENGL
#include "impl/graphics/opengl/RenderingManager.h"
#endif

#ifdef CHIMP_STB
#include "impl/graphics/images/stb/ImageLoader.h"
#endif

#ifdef CHIMP_ENET
#include "impl/networking/enet/ServerFactory.h"
#endif

#include "api/networking/PacketTypeRegistry.h"
#include "Loggers.h"

namespace Chimp {
	Engine::Engine() :
		m_AssetManager(*this),
		m_Window(CreateWindow()),
		m_ImageLoader(CreateImageLoader()),
		m_RenderingManager(CreateRenderingManager())
	{
		PacketTypeRegistry::RegisterChimpPacketTypes();

		Loggers::Main().Info("Initialized Chimp Engine!");
	}

	TimeManager& Engine::GetTimeManager()
	{
		return m_TimeManager;
	}

	IWindow& Engine::GetWindow()
	{
		return *m_Window;
	}

	IRenderingManager& Engine::GetRenderingManager()
	{
		return *m_RenderingManager;
	}

	AssetManager& Engine::GetAssetManager()
	{
		return m_AssetManager;
	}

	UpdateSubscriber& Engine::GetUpdateSubscriber()
	{
		return m_UpdateSubscriber;
	}

	SceneManager& Engine::GetSceneManager()
	{
		return *m_SceneManager;
	}

	std::unique_ptr<IServer> Engine::HostServer(const ConnectionInfo& serverInfo)
	{
		if (!serverInfo.IsValid()) {
			Loggers::Main().Error("Server info is invalid.");
			return nullptr;
		}

		std::unique_ptr<IServer> server = nullptr;
#ifdef CHIMP_ENET
		server = ENetServerFactory::CreateServer(serverInfo);
#endif
		if (server == nullptr) {
			Loggers::Main().Error("No networking implementation available.");
			return nullptr;
		}
		if (!server->IsValid()) {
			Loggers::Main().Error("Server is invalid, this means hosting failed.");
			return nullptr;
		}
		return std::move(server);
	}

	std::unique_ptr<IClient> Engine::ConnectToServer(const ConnectionInfo& serverInfo)
	{
		if (!serverInfo.IsValid()) {
			Loggers::Main().Error("Server info is invalid.");
			return nullptr;
		}

		std::unique_ptr<IClient> client = nullptr;
#ifdef CHIMP_ENET
		client = ENetServerFactory::CreateClient(serverInfo);
#endif
		if (client == nullptr) {
			Loggers::Main().Error("No networking implementation available.");
			return nullptr;
		}
		if (!client->IsValid()) {
			Loggers::Main().Error("Client is invalid, this means connecting failed.");
			return nullptr;
		}
		return std::move(client);
	}

	std::unique_ptr<Logger> Engine::CreateLogger(const std::string& name, LogLevel level)
	{
		return std::make_unique<Logger>(name, level);
	}

	std::unique_ptr<IWindow> Engine::CreateWindow() const
	{
		std::unique_ptr<IWindow> window = nullptr;
#ifdef CHIMP_GLFW
		window = std::make_unique<WindowGLFW>();
#endif
		if (window == nullptr) {
			Loggers::Main().Error("No window implementation available.");
			return nullptr;
		}
		window->GetImGuiHandler().Init(*window);
		return std::move(window);
	}

	std::unique_ptr<IRenderingManager> Engine::CreateRenderingManager() const
	{
		assert(m_ImageLoader);
		std::unique_ptr<IRenderingManager> renderingManager = nullptr;
#ifdef CHIMP_OPENGL
		renderingManager = std::make_unique<GL::RenderingManager>(*m_ImageLoader);
#endif

		if (!renderingManager) {
			Loggers::Main().Error("No rendering manager implementation available.");
			return nullptr;
		}

		renderingManager->GetRenderer().SetClearColor(0.0f, 0.0f, 0.0f);
		return renderingManager;
	}

	std::unique_ptr<IImageLoader> Engine::CreateImageLoader() const
	{
#ifdef CHIMP_STB
		return std::make_unique<STB::ImageLoader>();
#endif
		Loggers::Main().Error("No image loader implementation available.");
		return nullptr;
	}
}