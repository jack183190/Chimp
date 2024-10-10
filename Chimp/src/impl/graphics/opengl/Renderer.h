#pragma once

#include "api/graphics/IRenderer.h"

namespace Chimp::GL {
	class Renderer : public IRenderer
	{
	public:
		Renderer() = default;
		~Renderer() override = default;

		void Draw(const IElementArray& elementArray, const IShader& shader) const override;
	};
}