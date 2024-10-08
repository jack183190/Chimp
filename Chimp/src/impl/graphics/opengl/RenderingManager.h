#pragma once

#include "api/graphics/IRenderingManager.h"

namespace Chimp {
	class RenderingManager : public IRenderingManager {
	public:
		RenderingManager();
		~RenderingManager();

		std::unique_ptr<IBuffer> CreateBuffer(const Usage& usage, const BindTarget target) override;

		std::unique_ptr<IElementArrayLayout> CreateElementArrayLayout(
			const std::vector<ElementComponentLayout>& layouts) override;

		std::unique_ptr<IElementArray> CreateElementArray(
			std::unique_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			std::unique_ptr<IElementArrayLayout> layout) override;
	};
}