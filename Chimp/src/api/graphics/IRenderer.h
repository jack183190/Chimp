#pragma once

#include "buffers/IElementArray.h"
#include "shaders/IShader.h"
#include "Mesh.h"

namespace Chimp
{
	class IRenderer
	{
	protected:
		IRenderer() = default;
	public:
		virtual ~IRenderer() = default;

		void Draw(const Mesh& mesh) const;
		virtual void Draw(const Mesh::Section &meshSection) const = 0;
	};
}