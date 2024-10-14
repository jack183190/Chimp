#pragma once

#include "api/graphics/IRenderingManager.h"
#include "shaders/ShaderCompiler.h"
#include "impl/utils/NotNullHack.h"
#include "Renderer.h"

namespace Chimp::GL {
	class RenderingManager : public IRenderingManager {
	public:
		RenderingManager(IImageLoader& imageLoader);
		~RenderingManager();

		IRenderer& GetRenderer() override;

		std::unique_ptr<IBuffer> CreateBuffer(
			const Usage& usage,
			const BindTarget target
			) override;

		std::unique_ptr<IElementArrayLayout> CreateElementArrayLayout(
			const PrimitiveType primitivesType,
			const std::vector<ElementComponentLayout>& layouts
		) override;

		std::unique_ptr<IElementArray> CreateElementArray(
			std::shared_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			std::shared_ptr<IElementArrayLayout> layout
		) override;

		std::unique_ptr<IShader> CompileShader(const ShaderFilePaths& shaderFilePaths) override;

		std::unique_ptr<ITexture> CreateTexture(
			const TextureSlot slot,
			const TextureProperties& properties,
			const void* initialData) override;

	private:
		void InitOpenGL();

	private:
		NotNullHack<ShaderCompiler> m_ShaderCompiler;
		NotNullHack<Renderer> m_Renderer;
	};
}