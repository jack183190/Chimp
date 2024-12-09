#pragma once

#include "api/audio/ISound.h"
#include "PlayingAudio.h"

namespace Chimp {
	class Sound : public ISound {
	public:
		Sound(std::unique_ptr<IAudioImporter::AudioData> data) :
			ISound(std::move(data)) {
			alGenBuffers(1, &m_Id);
			alBufferData(m_Id, m_Data->Format, m_Data->Data.data(), m_Data->Data.size() * sizeof(short), m_Data->Frequency);
		}

		virtual ~Sound() {
			for (auto& audio : m_PlayingAudios) {
				audio->m_IsValid = false;
			}

			alDeleteBuffers(1, &m_Id);
		}

		std::shared_ptr<IPlayingAudio> Play(
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float gain
		) override {
			m_PlayingAudios.push_back(std::make_shared<PlayingAudio>(m_Id, position, velocity, pitch, gain));
			return m_PlayingAudios.back();
		}

		void Update() override {
			for (auto it = m_PlayingAudios.begin(); it != m_PlayingAudios.end();) {
				if (!(*it)->IsPlaying()) {
					it = m_PlayingAudios.erase(it);
				}
				else {
					++it;
				}
			}
		}

	private:
		ALuint m_Id;
		size_t m_Size;
	};
}