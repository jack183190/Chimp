#pragma once

#include "stdafx.h"

namespace Chimp {
	template <typename T>
	class IBuffer {
	protected:
		IBuffer() = default;

		~IBuffer() = default;

	public:
		// Bind the buffer
		virtual void Bind() = 0;

		T GetId() const { return m_Id; }
	protected:
		T m_Id;
	};
}