#include "api/graphics/buffers/IElementArray.h"

namespace Chimp {
	IElementArray::IElementArray(std::unique_ptr<IBuffer> vertexBuffer,
		std::unique_ptr<IBuffer> indexBuffer,
		std::unique_ptr<IElementArrayLayout> layout) :
		m_VertexBuffer(std::move(vertexBuffer)),
		m_IndexBuffer(std::move(indexBuffer)),
		m_Layout(std::move(layout))
	{
	}

	IBuffer& IElementArray::GetVertexBuffer()
	{
		return *m_VertexBuffer;
	}

	IBuffer& IElementArray::GetIndexBuffer()
	{
		return *m_IndexBuffer;
	}

	const IElementArrayLayout& IElementArray::GetElementLayout() const
	{
		return *m_Layout;
	}

	void IElementArray::SetLayout(std::unique_ptr<IElementArrayLayout> layout)
	{
		m_Layout = std::move(layout);
	}
}