#pragma once

#include "stdafx.h"
#include "buffers/Usage.h"
#include "buffers/IBuffer.h"
#include "buffers/IElementArrayLayout.h"
#include "buffers/IElementArray.h"
#include "shaders/IShader.h"
#include "shaders/ShaderTypes.h"
#include "PrimitiveType.h"
#include "IRenderer.h"
#include "api/graphics/images/IImageLoader.h"
#include "api/graphics/textures/ITexture.h"

namespace Chimp {
	class IRenderingManager {
	protected:
		IRenderingManager(IImageLoader& imageLoader);

	public:
		~IRenderingManager() = default;

	public:
		static constexpr TextureSlot CHIMP_TEXTURE_SLOT = 0;

	public:

		// Get the renderer
		[[nodiscard]] virtual IRenderer& GetRenderer() = 0;

		// Create a buffer which holds data on the GPU.
		// dataType, size, numElements can be left empty and will be updated once data is set.
		// dataType - the type of data in the buffer (e.g. float, int, etc.) (default float)
		// size - the size of the buffer in bytes (default 0)
		// numElements - the number of elements in the buffer (default 0)
		// usage - defines if the buffer can be updated and how often it will be updated (hint for optimization)
		// target - defines how the buffer will be used (e.g. vertex buffer, index buffer, etc.)
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(
			const GraphicsType dataType,
			const size_t size,
			const size_t numElements,
			const Usage& usage,
			const BindTarget target
		);
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(
			const Usage& usage,
			const BindTarget target
		) = 0;

		// Create an element array layout which defines how the data in an ElementArray is structured
		// primitivesType - the type of primitives in the ElementArray
		// layouts - a vector of ElementComponentLayouts which define the layout of each component of data in the ElementArray
		[[nodiscard]] virtual std::unique_ptr<IElementArrayLayout> CreateElementArrayLayout(
			const PrimitiveType primitivesType,
			const std::vector<ElementComponentLayout>& layouts) = 0;

		// Create an element array which contains a vertex and index buffer.
		// vertexBuffer - the buffer which contains the vertex data
		// indexBuffer - the buffer which contains the index data
		// layout - describes the layout of the vertex data
		[[nodiscard]] virtual std::unique_ptr<IElementArray> CreateElementArray(
			std::shared_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			std::shared_ptr<IElementArrayLayout> layout) = 0;

		// Compile a shader from source code
		// In chimp, a "shader" represents all the shaders in the pipeline (vertex, fragment, etc.)
		// Check Shader::IsValid() to see if the shader was compiled successfully
		[[nodiscard]] virtual std::unique_ptr<IShader> CompileShader(const ShaderFilePaths& shaderFilePaths) = 0;

		// Load an image from a file
		[[nodiscard]] virtual std::unique_ptr<IImageLoader::LoadedImage> LoadImage(const std::string& filePath);

		// Create a texture
		// The texture will be bound when it is constructed.
		// slot - the slot the texture is bound to
		// properties - the properties of the texture
		// initialData - the initial data to fill the texture with. This isn't owned by the texture, but it can be deleted after the constructor is called.
		[[nodiscard]] virtual std::unique_ptr<ITexture> CreateTexture(
			const TextureSlot slot,
			const TextureProperties& properties,
			const void* initialData) = 0;

		// Create a texture from an image. Most use cases can use CreateTextureFromImage instead.
		// The image can be destroyed after this function call, for example by moving a unique ptr into this function.
		// slot - the slot the texture is bound to
		// properties - the properties of the texture, you should ensure properties like width, height, and number of channels match the image
		// image - the image to create the texture from (make sure it is valid!)
		[[nodiscard]] virtual std::unique_ptr<ITexture> CreateCustomisedTextureFromImage(
			const TextureSlot slot,
			const TextureProperties& properties,
			const std::shared_ptr<IImageLoader::LoadedImage> image);

		// Create a texture from an image.
		// The texture will be bound to CHIMP_TEXTURE_SLOT.
		// Relevant properties will be extracted from the image, however some texture properties like filtering must be set by you (or use default values).
		// This function will return a nullptr if the image is not loaded successfully.
		// filePath - the path to the image file
		// properties - the properties of the texture
		[[nodiscard]] virtual std::unique_ptr<ITexture> CreateTextureFromImage(
			const std::string& filePath,
			const TextureProperties& properties = {}
		);

	protected:
		IImageLoader& m_ImageLoader;
	};
}