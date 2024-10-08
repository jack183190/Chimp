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
		virtual void Bind() = 0;

		// Unbind the element array
		virtual void Unbind() = 0;

		IBuffer& GetVertexBuffer();
		IBuffer& GetIndexBuffer();
		const IElementArrayLayout& GetElementLayout() const;
		void SetLayout(std::unique_ptr<IElementArrayLayout> layout);

	private:
		std::unique_ptr<IBuffer> m_VertexBuffer;
		std::unique_ptr<IBuffer> m_IndexBuffer;
		std::unique_ptr<IElementArrayLayout> m_Layout;
	};
}