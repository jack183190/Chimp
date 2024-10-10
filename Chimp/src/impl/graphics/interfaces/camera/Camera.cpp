#include "api/graphics/camera/Camera.h"

namespace Chimp {
	Camera::Camera() :
		m_Position(0.0f, 0.0f, 0.0f),
		m_UpVector(0.0f, 1.0f, 0.0f),
		m_ForwardVector(0.0f, 0.0f, 1.0f),
		m_RightVector()
	{
		UpdateViewMatrix();
	}

	void Camera::SetPosition(const Vector3f& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void Camera::SetUpVector(const Vector3f& up)
	{
		m_UpVector = VectorNormalized(up);
		m_RightVector = VectorNormalized(VectorCrossProduct(m_ForwardVector, m_UpVector));
		UpdateViewMatrix();
	}

	void Camera::SetForwardVector(const Vector3f& forward)
	{
		m_ForwardVector = VectorNormalized(forward);
		m_RightVector = VectorNormalized(VectorCrossProduct(m_ForwardVector, m_UpVector));
		UpdateViewMatrix();
	}

	const Vector3f& Camera::GetPosition() const
	{
		return m_Position;
	}

	const Vector3f Camera::GetTarget() const
	{
		return m_Position + m_ForwardVector;
	}

	const Vector3f& Camera::GetUpVector() const
	{
		return m_UpVector;
	}

	const Vector3f& Camera::GetForwardVector() const
	{
		return m_ForwardVector;
	}

	const Vector3f& Camera::GetRightVector() const
	{
		return m_RightVector;
	}

	const CameraMatrices& Camera::GetCameraMatrices() const
	{
		return m_CameraMatrices;
	}

	void Camera::UpdateViewMatrix()
	{
		m_CameraMatrices.SetViewMatrix(CreateViewMatrix(m_Position, m_Position + m_ForwardVector, m_UpVector));
	}
}