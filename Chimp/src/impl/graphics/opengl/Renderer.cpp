#pragma once

#include "Renderer.h"
#include "Translations.h"

namespace Chimp::GL {
	constexpr void* INDICES_BOUND_IN_VAO = nullptr;

	void Renderer::Draw(const Mesh::Section& meshSection) const
	{
		meshSection.Shader->Bind();
		meshSection.ElementArray->Bind();

		glDrawElements(
			PrimitiveTypeTranslation(meshSection.ElementArray->GetPrimitiveType()),
			meshSection.ElementArray->GetIndexCount(),
			TypeTranslation(meshSection.ElementArray->GetIndexType()),
			INDICES_BOUND_IN_VAO
		);

		meshSection.ElementArray->Unbind();
		meshSection.Shader->Unbind();
	}
}