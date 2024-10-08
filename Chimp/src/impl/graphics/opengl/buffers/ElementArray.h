#pragma once

#include "api/graphics/buffers/IElementArray.h"

namespace Chimp::GL {
	class ElementArray : public IElementArray {
	public:
		ElementArray(
			std::unique_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			std::unique_ptr<IElementArrayLayout> layout);
		~ElementArray() = default;

		void Bind() override;

		void Unbind() override;

	private:
		GLuint m_Id;
	};
}