#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"
#include "IAudioImporter.h"

namespace Chimp {
	class IPlayingAudio;

	class ISound {
	protected:
		ISound(std::unique_ptr<IAudioImporter::AudioData> data) :
			m_Data(std::move(data))
		{
		}

	public:
		virtual ~ISound() = default;

		// Play the sound
		// position - The position of the sound
		// velocity - The velocity of the sound
		// pitch - The pitch of the sound
		// gain - The gain of the sound
		virtual std::shared_ptr<IPlayingAudio> Play(
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float gain
		) = 0;

	protected:
		virtual void Update() = 0;

	protected:
		std::unique_ptr<IAudioImporter::AudioData> m_Data;
		std::vector<std::shared_ptr<IPlayingAudio>> m_PlayingAudios;
	};
}