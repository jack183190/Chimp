#include "api/utils/Maths.h"

namespace Chimp {
	Vector3f VectorCrossProduct(const Vector3f& a, const Vector3f& b)
	{
		return glm::cross(a, b);
	}

	Vector3f VectorNormalized(const Vector3f& a)
	{
		return glm::normalize(a);
	}

	Matrix CreateIdentityMatrix()
	{
		return glm::identity<Matrix>();
	}

	Matrix CreateTranslationMatrix(Vector3f translation)
	{
		return glm::translate(CreateIdentityMatrix(), translation);
	}

	Matrix CreateScaleMatrix(Vector3f scale)
	{
		return glm::scale(CreateIdentityMatrix(), scale);
	}

	std::array<Vector3f, 3> GetForwardUpRightVectors(const Vector3f yawPitchRoll)
	{
		// https://chatgpt.com/c/670ba852-a8f4-8003-844d-82eb84a0a3ec
		// Extract yaw, pitch, and roll
		const float yaw = yawPitchRoll.x;
		const float pitch = yawPitchRoll.y;
		const float roll = yawPitchRoll.z;

		// Pre-compute trigonometric functions
		const float cosYaw = glm::cos(yaw);
		const float sinYaw = glm::sin(yaw);
		const float cosPitch = glm::cos(pitch);
		const float sinPitch = glm::sin(pitch);
		const float cosRoll = glm::cos(roll);
		const float sinRoll = glm::sin(roll);

		// Calculate forward vector
		Vector3f forward;
		forward.x = cosPitch * sinYaw;
		forward.y = sinPitch;
		forward.z = cosPitch * -cosYaw;

		// Calculate right vector
		Vector3f right;
		right.x = cosYaw * cosRoll - sinYaw * sinPitch * sinRoll;
		right.y = sinPitch;
		right.z = sinYaw * cosRoll + sinPitch * sinRoll * cosYaw;

		Vector3f up = VectorCrossProduct(right, forward);

		// Normalize the vectors
		forward = VectorNormalized(forward);
		up = VectorNormalized(up);
		right = VectorNormalized(right);

		return { forward, up, right };
	}

	Matrix CreateViewMatrix(Vector3f position, Vector3f target, Vector3f up)
	{
		return glm::lookAt(position, target, up);
	}

	Matrix CreateOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}
}
