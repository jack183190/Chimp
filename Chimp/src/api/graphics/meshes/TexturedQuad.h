#pragma once

#include "api/graphics/IRenderingManager.h"

namespace Chimp {
	class TexturedQuad : public Mesh {
	public:
		TexturedQuad() = delete;

		// Create a textured quad mesh parallel to the XY plane centered at the origin.
		// Vertex layout:
		// 0: Position (3 floats)
		// 1: Texture coordinates (2 floats)
		// 
		// renderingManager - The rendering manager
		// shader - The shader to use
		static std::unique_ptr<Mesh> Create(
			IRenderingManager& renderingManager,
			std::shared_ptr<IShader> shader
		);
	};
}