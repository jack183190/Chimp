#pragma once

#include "api/graphics/buffers/Usage.h"
#include "stdafx.h"

namespace Chimp::GL {
	[[nodiscard]] static constexpr GLenum BindTargetTranslation(const BindTarget target) {
		switch (target) {
		case BindTarget::VERTEX_BUFFER:
			return GL_ARRAY_BUFFER;
		case BindTarget::INDEX_BUFFER:
			return GL_ELEMENT_ARRAY_BUFFER;
		default:
			__assume(false);
		}
	}

	[[nodiscard]] static constexpr GLenum UsageTranslation(const Usage& usage) {
		if (usage.Access == Usage::Access::READ) {
			if (usage.UpdateFrequency == Usage::UpdateFrequency::VERY_OCCASIONAL) {
				return GL_STATIC_READ;
			}
			else if (usage.UpdateFrequency == Usage::UpdateFrequency::OCCASIONAL) {
				return GL_DYNAMIC_READ;
			}
			else {
				return GL_STREAM_READ;
			}
		}
		else if (usage.Access == Usage::Access::WRITE) {
			if (usage.UpdateFrequency == Usage::UpdateFrequency::VERY_OCCASIONAL) {
				return GL_STATIC_DRAW;
			}
			else if (usage.UpdateFrequency == Usage::UpdateFrequency::OCCASIONAL) {
				return GL_DYNAMIC_DRAW;
			}
			else {
				return GL_STREAM_DRAW;
			}
		}
		else {
			if (usage.UpdateFrequency == Usage::UpdateFrequency::VERY_OCCASIONAL) {
				return GL_STATIC_COPY;
			}
			else if (usage.UpdateFrequency == Usage::UpdateFrequency::OCCASIONAL) {
				return GL_DYNAMIC_COPY;
			}
			else {
				return GL_STREAM_COPY;
			}
		}
	}
}