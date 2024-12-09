#pragma once

#include "stdafx.h"

namespace Chimp {
	class Sound;
	class IPlayingAudio {
		friend class Sound;
	protected:
		IPlayingAudio(
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float gain
		) :
			m_Position(position),
			m_Velocity(velocity),
			m_Pitch(pitch),
			m_Gain(gain)
		{}
	public:
		virtual ~IPlayingAudio() = default;

		virtual bool IsPlaying() const = 0;
		virtual bool IsValid() const = 0;

	private:
		Vector3f m_Position;
		Vector3f m_Velocity;
		float m_Pitch;
		float m_Gain;
		bool m_IsValid = true;
	};
}