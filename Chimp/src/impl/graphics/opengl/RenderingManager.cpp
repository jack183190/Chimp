#include "RenderingManager.h"
#include "buffers/Buffer.h"
#include "buffers/ElementArray.h"
#include "buffers/ElementArrayLayout.h"
#include "shaders/Shader.h"
#include "textures/Texture.h"
#include "Loggers.h"

// thank u chatgpt
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// Ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

namespace Chimp::GL {
	RenderingManager::RenderingManager(IImageLoader& imageLoader) : 
		IRenderingManager(imageLoader)
	{
		// Initialise OpenGL
		InitOpenGL();

		// Shader compiler
		m_ShaderCompiler = std::make_unique<ShaderCompiler>();

		// Renderer
		m_Renderer = std::make_unique<GL::Renderer>();
	}

	RenderingManager::~RenderingManager() {

	}

	IRenderer& RenderingManager::GetRenderer() const
	{
		return *m_Renderer;
	}

	std::unique_ptr<IBuffer> RenderingManager::CreateBuffer(
		const Usage& usage,
		const BindTarget target) const
	{
		return std::make_unique<GL::Buffer>(usage, target);
	}

	std::unique_ptr<IElementArrayLayout> RenderingManager::CreateElementArrayLayout(
		const PrimitiveType primitivesType,
		const std::vector<ElementComponentLayout>& layouts) const
	{
		return std::make_unique<GL::ElementArrayLayout>(primitivesType, layouts);
	}

	std::unique_ptr<IElementArray> RenderingManager::CreateElementArray(
		std::shared_ptr<IBuffer> vertexBuffer,
		std::unique_ptr<IBuffer> indexBuffer,
		GraphicsType indexType,
		std::shared_ptr<IElementArrayLayout> layout) const
	{
		return std::make_unique<GL::ElementArray>(
			std::move(vertexBuffer),
			std::move(indexBuffer),
			indexType,
			std::move(layout)
		);
	}

	std::unique_ptr<IShader> RenderingManager::CompileShader(const ShaderFilePaths& shaderFilePaths) const
	{
		return std::make_unique<GL::Shader>(
			shaderFilePaths,
			*m_ShaderCompiler
		);
	}

	std::unique_ptr<ITexture> RenderingManager::CreateTexture(const TextureSlot slot, const TextureProperties& properties, const void* initialData) const
	{
		return std::make_unique<GL::Texture>(slot, properties, initialData);
	}

	void RenderingManager::InitOpenGL()
	{
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
			std::cerr << "Failed to initialize GLEW." << std::endl;
			std::stringstream ss;
			ss << "Failed to initialize GLEW: " << glewGetErrorString(err);
			Loggers::Rendering().Error(ss);
			exit(-1);
		}
		else {
			Loggers::Rendering().Info("GLEW initialized.");
		}

#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glDebugOutput, nullptr);
#endif

		// Depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glCullFace(GL_BACK);

		Loggers::Rendering().Info("Initialized OpenGL Renderer.");
		Loggers::Rendering().Info(" OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));
		Loggers::Rendering().Info(" GLSL Version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
		Loggers::Rendering().Info(" Vendor: " + std::string((const char*)glGetString(GL_VENDOR)));
		Loggers::Rendering().Info(" Renderer: " + std::string((const char*)glGetString(GL_RENDERER)));
	}
}