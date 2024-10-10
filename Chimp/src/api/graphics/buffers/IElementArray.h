#pragma once

#include "IBuffer.h"
#include "api/graphics/buffers/IElementArrayLayout.h"

namespace Chimp {
	class IElementArray {
	protected:
		IElementArray(std::unique_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			std::unique_ptr<IElementArrayLayout> layout);
	public:
		~IElementArray() = default;

	public:
		// Bind the element array
		virtual void Bind() const = 0;

		// Unbind the element array
		virtual void Unbind() const = 0;

		IBuffer& GetVertexBuffer();
		IBuffer& GetIndexBuffer();
		const IElementArrayLayout& GetElementLayout() const;
		void SetLayout(std::unique_ptr<IElementArrayLayout> layout);

		// Get the number of indices in the element array
		[[nodiscard]] unsigned int GetIndexCount() const;

		// Get the data type of the indices in the element array
		[[nodiscard]] GraphicsType GetIndexType() const;

		// Get the primitive type of the element array
		[[nodiscard]] PrimitiveType GetPrimitiveType() const;

	private:
		std::unique_ptr<IBuffer> m_VertexBuffer;
		std::unique_ptr<IBuffer> m_IndexBuffer;
		std::unique_ptr<IElementArrayLayout> m_Layout;
	};
}