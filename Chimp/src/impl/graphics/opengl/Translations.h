#pragma once

#include "api/graphics/buffers/Usage.h"
#include "api/graphics/GraphicsType.h"
#include "api/graphics/shaders/ShaderTypes.h"
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

	[[nodiscard]] static constexpr GLenum ShaderTypeTranslation(const ShaderType shaderType) {
		switch (shaderType) {
		case ShaderType::VERTEX:
			return GL_VERTEX_SHADER;
		case ShaderType::FRAGMENT:
			return GL_FRAGMENT_SHADER;
		default:
			__assume(false);
		}
	}

	[[nodiscard]] static GLenum TypeTranslation(const GraphicsType& type) {
		if (type == GraphicsType::FLOAT) {
			return GL_FLOAT;
		}
		else if (type == GraphicsType::UNSIGNED_INT) {
			return GL_UNSIGNED_INT;
		}
		__assume(false);
	}

	[[nodiscard]] static constexpr GLenum UsageTranslation(const Usage& usage) {
		if (usage.Access == Usage::Access::CPU_READ) {
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
		else if (usage.Access == Usage::Access::CPU_WRITE) {
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