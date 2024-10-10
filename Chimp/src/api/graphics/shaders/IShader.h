#pragma once

#include "stdafx.h"

namespace Chimp {
	// Represents all the shaders in a pipeline
	// Vertex, fragment, etc
	class IShader {
		DISABLE_COPY(IShader);

	protected:
		IShader() = default;

	public:
		virtual ~IShader() = default;

		// Bind the shader
		virtual void Bind() const = 0;

		// Unbind the shader
		virtual void Unbind() const = 0;

		// Check if the shader is valid (all shaders compiled successfully)
		virtual bool IsValid() const = 0;
	};
}