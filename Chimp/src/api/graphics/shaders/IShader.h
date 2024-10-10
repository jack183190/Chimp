#pragma once

#include "stdafx.h"
#include "IShaderBuffers.h"

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

		// Get the shader buffers
		virtual [[nodiscard]] IShaderBuffers& GetShaderBuffers() = 0;

		// Bind the buffers to the shader
		// this must be called whenever the data (or subdata) in the IBuffer has changed
		virtual void UpdateShaderBuffer(IShaderBuffers::Id id) const = 0;
	};
}