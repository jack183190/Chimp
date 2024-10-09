#pragma once

#include "stdafx.h"

namespace Chimp {
	class IShader {
		DISABLE_COPY(IShader);

	protected:
		IShader() = default;

	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
	};
}