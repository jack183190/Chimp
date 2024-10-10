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

namespace Chimp {
	class IRenderingManager {
	protected:
		IRenderingManager() = default;

	public:
		~IRenderingManager() = default;

	public:
		// Get the renderer
		[[nodiscard]] virtual IRenderer& GetRenderer() = 0;

		// Create a buffer which holds data on the GPU
		// usage - defines if the buffer can be updated and how often it will be updated (hint for optimization)
		// target - defines how the buffer will be used (e.g. vertex buffer, index buffer, etc.)
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(const Usage& usage, const BindTarget target) = 0;

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
		[[nodiscard]] virtual std::unique_ptr<IShader> CompileShader(const ShaderFilePaths &shaderFilePaths) = 0;
	};
}