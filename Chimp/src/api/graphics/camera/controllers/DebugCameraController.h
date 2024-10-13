#pragma once

#include "../Camera.h"
#include "api/window/InputManager.h"

namespace Chimp {
	class DebugCameraController {
	public:
		DebugCameraController(Camera& camera, InputManager& inputManager);

		void OnUpdate(float deltaTime);

	private:
		Camera& m_Camera;
		InputManager& m_InputManager;
		Vector3f m_YawPitchRoll = Vector3f(PI, 0, 0);
		static constexpr float MOVEMENT_SPEED = 25.0f;
		static constexpr float ROTATION_SPEED = 0.25f;
	};
}