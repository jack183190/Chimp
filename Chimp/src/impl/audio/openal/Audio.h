#pragma once

#include "api/audio/IAudio.h"
#include "Loggers.h"

namespace Chimp {
	class Audio : public IAudio {
	public:
		Audio() :
			IAudio(),
			m_IsValid(false) 
		{
			m_Device = alcOpenDevice(nullptr);
			if (!m_Device) {
				Loggers::Audio().Error("Failed to open audio device!");
				return;
			}

			m_Context = alcCreateContext(m_Device, nullptr);
			if (!m_Context || alcMakeContextCurrent(m_Context) == ALC_FALSE) {
				Loggers::Audio().Error("Failed to create audio context!");
				if (m_Context) alcDestroyContext(m_Context);
				alcCloseDevice(m_Device);
				return;
			}

			m_IsValid = true;
		}

		~Audio() {
			alcMakeContextCurrent(nullptr);

			if (m_Context) alcDestroyContext(m_Context);
			if (m_Device) alcCloseDevice(m_Device);
		}

		bool IsValid() const {
			return m_IsValid;
		}

	private:
		ALCdevice* m_Device;
		ALCcontext* m_Context;
		bool m_IsValid;
	};
}