#pragma once

#include "preprocessor/CopyAndMove.h"

namespace Chimp {
	struct GraphicsType {
		DISABLE_MOVE(GraphicsType);
	private:
		GraphicsType(unsigned int size);
		
	public:
		~GraphicsType() = default;

		GraphicsType(const GraphicsType& other) = default;

		GraphicsType& operator=(const GraphicsType& other) = default;

	public:
		bool operator==(const GraphicsType& other) const;

	public:
		unsigned int Size; // size in bytes of this type

	private:
		unsigned int m_Id;

	public:
		static const GraphicsType FLOAT;
		static const GraphicsType UNSIGNED_INT;

	private:
		static unsigned int NumberTypes;
	};
}