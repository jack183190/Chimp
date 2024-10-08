#pragma once

#include "stdafx.h"
#include "buffers/Usage.h"
#include "buffers/IBuffer.h"
#include "buffers/IElementArrayLayout.h"
#include "buffers/IElementArray.h"

namespace Chimp {
	class IRenderingManager {
	protected:
		IRenderingManager() = default;

	public:
		~IRenderingManager() = default;

	public:
		// Create a buffer which holds data on the GPU
		// usage - defines if the buffer can be updated and how often it will be updated (hint for optimization)
		// target - defines how the buffer will be used (e.g. vertex buffer, index buffer, etc.)
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(const Usage& usage, const BindTarget target) = 0;

		// Create an element array layout which defines how the data in an ElementArray is structured
		// layouts - a vector of ElementComponentLayouts which define the layout of each component of data in the ElementArray
		[[nodiscard]] virtual std::unique_ptr<IElementArrayLayout> CreateElementArrayLayout(
			const std::vector<ElementComponentLayout>& layouts) = 0;

		// Create an element array which contains a vertex and index buffer.
		// vertexBuffer - the buffer which contains the vertex data
		// indexBuffer - the buffer which contains the index data
		// layout - describes the layout of the vertex data
		[[nodiscard]] virtual std::unique_ptr<IElementArray> CreateElementArray(
			std::unique_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			std::unique_ptr<IElementArrayLayout> layout) = 0;
	};
}