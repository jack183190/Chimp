#pragma once

#include "buffers/IElementArray.h"
#include "shaders/IShader.h"

namespace Chimp
{
	class IRenderer
	{
	protected:
		IRenderer() = default;
	public:
		virtual ~IRenderer() = default;

		virtual void Draw(const IElementArray& elementArray, const IShader& shader) const = 0;
	};
}