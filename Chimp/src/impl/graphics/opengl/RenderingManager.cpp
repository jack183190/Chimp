#include "RenderingManager.h"
#include "buffers/Buffer.h"

namespace Chimp {
	RenderingManager::RenderingManager() {
	}

	RenderingManager::~RenderingManager() {
	}

	std::unique_ptr<IBuffer> RenderingManager::CreateBuffer(const Usage& usage, const BindTarget target)
	{
		return std::make_unique<GL::Buffer>(usage, target);
	}
}