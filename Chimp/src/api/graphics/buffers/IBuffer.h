#pragma once

#include "stdafx.h"
#include "Usage.h"

namespace Chimp {
	class IBuffer {
	protected:
		IBuffer() = default;

	public:
		~IBuffer() = default;

	public:
		// Bind the buffer
		virtual void Bind() = 0;

		// Change the bind target, this will not take effect until Bind() is called
		virtual void SetBindTarget(BindTarget target) = 0;

		// Set the data of the buffer, this will call Bind()
		void SetData(const void* data, size_t size) {
			Bind();
			SetDataBindless(data, size);
		}

		// Set the data of the buffer, this will not call Bind()
		virtual void SetDataBindless(const void* data, size_t size) = 0;

		// Set the sub data of the buffer, this will call Bind()
		void SetSubData(const void* data, size_t size, size_t offset) {
			Bind();
			SetSubDataBindless(data, size, offset);
		}

		// Set the sub data of the buffer, this will not call Bind()
		virtual void SetSubDataBindless(const void* data, size_t size, size_t offset) = 0;
	};
}