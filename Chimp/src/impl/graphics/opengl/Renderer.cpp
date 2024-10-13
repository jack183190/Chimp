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

	void Renderer::SetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void Renderer::StartDrawing() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}