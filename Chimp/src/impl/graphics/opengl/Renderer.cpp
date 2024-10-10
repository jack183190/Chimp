#pragma once

#include "Renderer.h"
#include "Translations.h"

namespace Chimp::GL {
	void Renderer::Draw(const IElementArray& elementArray, const IShader& shader) const
	{
		shader.Bind();
		elementArray.Bind();

		constexpr void* INDICES_BOUND_IN_VAO = nullptr;

		glDrawElements(
			PrimitiveTypeTranslation(elementArray.GetPrimitiveType()),
			elementArray.GetIndexCount(),
			TypeTranslation(elementArray.GetIndexType()),
			INDICES_BOUND_IN_VAO
		);

		elementArray.Unbind();
		shader.Unbind();
	}
}