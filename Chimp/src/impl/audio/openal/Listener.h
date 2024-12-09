#pragma once

#include "api/audio/IListener.h"

namespace Chimp {
	class Listener : public Chimp::IListener {
	public:
		Listener() = default;
		~Listener() = default;

		void SetPosition(Vector3f position) override {
			Chimp::IListener::SetPosition(position);
			alListener3f(AL_POSITION, position.x, position.y, position.z);
		}

		void SetVelocity(Vector3f velocity) override {
			Chimp::IListener::SetVelocity(velocity);
			alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		}

		void SetOrientation(Vector3f forward, Vector3f up) override {
			Chimp::IListener::SetOrientation(forward, up);
			float orientation[6] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
			alListenerfv(AL_ORIENTATION, orientation);
		}
	};
}