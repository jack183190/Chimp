#pragma once

#include "stdafx.h"

namespace Chimp {
	struct RawArray {
		DISABLE_COPY(RawArray);
		RawArray() = default;
		RawArray(RawArray&& toMove) noexcept;
		RawArray& operator=(RawArray&& toMove) noexcept;
		~RawArray();

		size_t Size;
		size_t NumberElements;
		void* Data;
	};
}