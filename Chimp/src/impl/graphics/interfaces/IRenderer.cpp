#include "api/graphics/IRenderer.h"

void Chimp::IRenderer::Draw(const Mesh& mesh) const
{
	for (const auto& section : mesh)
	{
		Draw(section);
	}
}
