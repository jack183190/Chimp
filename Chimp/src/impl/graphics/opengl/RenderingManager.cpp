#include "RenderingManager.h"
#include "buffers/Buffer.h"
#include "buffers/ElementArray.h"
#include "buffers/ElementArrayLayout.h"

namespace Chimp::GL {
	RenderingManager::RenderingManager() {
		// Initialise OpenGL
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
			std::cerr << "Failed to initialise GLEW." << std::endl;
			exit(-1);
		}

		std::cout << "Initialised OpenGL Renderer:" << std::endl;
		std::cout << " OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		// Shader compiler
		m_ShaderCompiler = std::make_unique<ShaderCompiler>();
	}

	RenderingManager::~RenderingManager() {
	}

	std::unique_ptr<IBuffer> RenderingManager::CreateBuffer(const Usage& usage, const BindTarget target)
	{
		return std::make_unique<GL::Buffer>(usage, target);
	}

	std::unique_ptr<IElementArrayLayout> RenderingManager::CreateElementArrayLayout(const std::vector<ElementComponentLayout>& layouts)
	{
		return std::make_unique<GL::ElementArrayLayout>(layouts);
	}

	std::unique_ptr<IElementArray> RenderingManager::CreateElementArray(std::unique_ptr<IBuffer> vertexBuffer, std::unique_ptr<IBuffer> indexBuffer, std::unique_ptr<IElementArrayLayout> layout)
	{
		return std::make_unique<GL::ElementArray>(
			std::move(vertexBuffer),
			std::move(indexBuffer),
			std::move(layout)
		);
	}
}