#pragma once

#include "stdafx.h"
#include "buffers/Usage.h"

namespace Chimp {
	class IBuffer;

	class IRenderingManager {
	protected:
		IRenderingManager() = default;

		~IRenderingManager() = default;

	public:
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(const Usage& usage, const BindTarget target) = 0;
	};
}