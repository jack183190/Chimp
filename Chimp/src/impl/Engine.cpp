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

namespace Chimp {
	Engine::Engine() :
		m_AssetManager(*this),
		m_Window(CreateWindow()),
		m_ImageLoader(CreateImageLoader()),
		m_RenderingManager(CreateRenderingManager())
	{
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

	std::unique_ptr<IServer> Engine::ConnectOrHostServer(const ServerInfo& serverInfo)
	{
		if (!serverInfo.IsValid()) {
			std::cerr << "Server info is invalid." << std::endl;
			return nullptr;
		}

		std::unique_ptr<IServer> server = nullptr;
#ifdef CHIMP_ENET
		server = ENetServerFactory::CreateServer(serverInfo);
#endif
		if (server == nullptr) {
			std::cerr << "No server implementation available." << std::endl;
			return nullptr;
		}
		if (!server->IsValid()) {
			std::cerr << "Server is invalid, this means hosting or connecting failed." << std::endl;
			return nullptr;
		}
		return std::move(server);
	}

	std::unique_ptr<IWindow> Engine::CreateWindow() const
	{
#ifdef CHIMP_GLFW
		return std::make_unique<WindowGLFW>();
#endif
		std::cerr << "No window implementation available." << std::endl;
		return nullptr;
	}

	std::unique_ptr<IRenderingManager> Engine::CreateRenderingManager() const
	{
		assert(m_ImageLoader);
		std::unique_ptr<IRenderingManager> renderingManager = nullptr;
#ifdef CHIMP_OPENGL
		renderingManager= std::make_unique<GL::RenderingManager>(*m_ImageLoader);
#endif

		if (!renderingManager) {
			std::cerr << "No rendering manager implementation available." << std::endl;
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
		std::cerr << "No image loader implementation available." << std::endl;
		return nullptr;
	}
}