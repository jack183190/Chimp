#pragma once

#include "stdafx.h"
#include "api/graphics/GraphicsType.h"

namespace Chimp {
	// Represents a single component of a vertex
	// this could be position, uv coordinates, etc
	// multiple of these will make up a vertex
	struct ElementComponentLayout {
		unsigned int Count; // number of elements
		GraphicsType Type; // type of elements
		bool Normalized; // normalize the data (map unsigned integers to floats in range 0 to 1) (ints mapped to -1 to 1)
	};

	class IElementArrayLayout {
	protected:
		IElementArrayLayout(const std::vector<ElementComponentLayout>& layouts);

	public:
		virtual ~IElementArrayLayout() = default;

		virtual void Bind() const = 0;

	private:
		static unsigned int CalculateStride(const std::vector<ElementComponentLayout>& layouts);

	protected:
		const std::vector<ElementComponentLayout> Layouts;
		const unsigned int Stride;
	};
}