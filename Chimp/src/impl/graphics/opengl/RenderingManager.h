#pragma once

#include "api/graphics/IRenderingManager.h"

namespace Chimp {
	class RenderingManager : public IRenderingManager {
	public:
		RenderingManager();
		~RenderingManager();

		std::unique_ptr<IBuffer> CreateBuffer(const Usage& usage, const BindTarget target) override;
	};
}