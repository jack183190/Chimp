#include "ElementArray.h"

namespace Chimp::GL {
	ElementArray::ElementArray(std::unique_ptr<IBuffer> vertexBuffer,
		std::unique_ptr<IBuffer> indexBuffer,
		std::unique_ptr<IElementArrayLayout> layout) :
		IElementArray(std::move(vertexBuffer),
			std::move(indexBuffer),
			std::move(layout))
	{
		glGenVertexArrays(1, &m_Id);
		Bind();

		GetVertexBuffer().Bind();
		GetIndexBuffer().Bind();
		GetElementLayout().Bind();

		Unbind();
	}

	void ElementArray::Bind()
	{
		glBindVertexArray(m_Id);
	}

	void ElementArray::Unbind()
	{
		glBindVertexArray(0);
	}
}