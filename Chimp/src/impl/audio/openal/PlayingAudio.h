#pragma once

#include "api/audio/IPlayingAudio.h"

namespace Chimp {
	class PlayingAudio : public IPlayingAudio {
	public:
		PlayingAudio(
			ALuint soundId,
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float gain
		) :
			IPlayingAudio(position, velocity, pitch, gain),
			m_SoundId(soundId)
		{
			alGenSources(1, &m_SourceId);

			alSourcei(m_SourceId, AL_BUFFER, m_SoundId);

			alSource3f(m_SourceId, AL_POSITION, position.x, position.y, position.z);
			alSource3f(m_SourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
			alSourcef(m_SourceId, AL_PITCH, pitch);
			alSourcef(m_SourceId, AL_GAIN, gain);
			alSourcei(m_SourceId, AL_LOOPING, AL_FALSE);

			alSourcePlay(m_SourceId);
		}

		~PlayingAudio() {
			alSourceStop(m_SourceId);
			alDeleteSources(1, &m_SourceId);
		}

		bool IsPlaying() const override {
			ALint state;
			alGetSourcei(m_SourceId, AL_SOURCE_STATE, &state);
			return IsValid() && state == AL_PLAYING;
		}

		bool IsValid() const override {
			return m_SoundId != 0 && m_SourceId != 0;
		}
		
	private:
		ALuint m_SoundId;
		ALuint m_SourceId;
	};
}