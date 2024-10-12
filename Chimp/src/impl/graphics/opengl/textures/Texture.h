#pragma once

#include "api/graphics/textures/ITexture.h"

namespace Chimp::GL {
	class Texture : public ITexture {
	public:
		Texture(const TextureSlot slot,
			const TextureProperties& properties,
			const void* initialData);
		~Texture() override;

		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int m_TextureId;
	};
}