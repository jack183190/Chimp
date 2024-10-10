#pragma once

#include "CameraMatrices.h"
#include "ICamera.h"

namespace Chimp {
	// Represents a camera in 3D space
	// if this is too high level, see CameraMatrices
	// Updating any property of the camera will automatically update the view / projection matrix
	class Camera : public ICamera {
	public:
		// Create a camera positioned at (0, 0, 0) where +z is forward and +y is up
		Camera();
		~Camera() = default;

		// Set the position of the camera
		void SetPosition(const Vector3f& position);

		// Set the up vector of the camera, this will normalise (a copy of) the input vector and recalculate the right vector
		void SetUpVector(const Vector3f& up);

		// Set the forward vector of the camera, this will normalise (a copy of) the input vector and recalculate the right vector
		void SetForwardVector(const Vector3f& forward);

		// Get the position of the camera
		[[nodiscard]] const Vector3f& GetPosition() const;

		// Equivalent to position + forward vector, get the position the camera is looking at
		[[nodiscard]] const Vector3f GetTarget() const;

		// Get the up vector of the camera
		[[nodiscard]] const Vector3f& GetUpVector() const;

		// Get the forward vector of the camera
		[[nodiscard]] const Vector3f& GetForwardVector() const;

		// Get the right vector of the camera
		[[nodiscard]] const Vector3f& GetRightVector() const;

		// Get the camera matrices
		[[nodiscard]] const CameraMatrices& GetCameraMatrices() const override;

	private:
		// Update the view matrix based on the current camera properties
		void UpdateViewMatrix();

	private:
		CameraMatrices m_CameraMatrices;

		Vector3f m_Position;
		Vector3f m_UpVector;
		Vector3f m_ForwardVector;
		Vector3f m_RightVector;
	};
}