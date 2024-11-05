#pragma once

#include <array>
#include <../vendor/glm/glm/glm.hpp>
#include <../vendor/glm/glm/gtc/matrix_transform.hpp>
#include <../vendor/glm/glm/gtc/type_ptr.hpp>
#include <../vendor/glm/glm/gtc/constants.hpp>

// Done so glm can be swapped out by just changing this file rather than the whole project

namespace Chimp {
	typedef glm::vec2 Vector2f;
	typedef glm::vec3 Vector3f;
	typedef glm::vec4 Vector4f;

	typedef glm::vec<2, int> Vector2i;
	typedef glm::vec<3, int> Vector3i;
	typedef glm::vec<4, int> Vector4i;

	typedef glm::mat4x4 Matrix;

	constexpr float PI = glm::pi<float>();

	// Cross product of two vectors
	[[nodiscard]] Vector3f VectorCrossProduct(const Vector3f& a, const Vector3f& b);

	// Normalize a vector
	[[nodiscard]] Vector3f VectorNormalized(const Vector3f& a);

	// Create an identity matrix
	[[nodiscard]] Matrix CreateIdentityMatrix();

	// Create a translation matrix
	// translation - The translation vector
	[[nodiscard]] Matrix CreateTranslationMatrix(Vector3f translation);

	// Create a scale matrix
	// scale - The scale vector
	[[nodiscard]] Matrix CreateScaleMatrix(Vector3f scale);

	// Create rotation matrix from yaw, pitch and roll (in radians)
	// y, x, z axis rotation
	[[nodiscard]] Matrix CreateRotationMatrixYawPitchRoll(Vector3f yawPitchRoll);

	// Get the forward, up and right vectors from a yaw, pitch and roll (in radians)
	[[nodiscard]] std::array<Vector3f, 3> GetForwardUpRightVectors(const Vector3f yawPitchRoll);

	// Create a view matrix
	// position - The position of the camera
	// target - The point the camera is looking at
	// up - The up vector of the camera
	[[nodiscard]] Matrix CreateViewMatrix(Vector3f position, Vector3f target, Vector3f up = Vector3f(0.0f, 1.0f, 0.0f));

	// Create an orthographic projection matrix
	// left - The coordinate of the left vertical clipping plane. This defines the minimum x-coordinate visible in the orthographic projection.
	// right - The coordinate of the right vertical clipping plane. This defines the maximum x-coordinate visible in the orthographic projection.
	// bottom - The coordinate of the bottom horizontal clipping plane. This defines the minimum y-coordinate visible in the orthographic projection.
	// top - The coordinate of the top horizontal clipping plane. This defines the maximum y-coordinate visible in the orthographic projection.
	// zNear - The Z value of the near clipping plane. This defines the minimum Z-coordinate visible in the orthographic projection.
	// zFar - The Z value of the far clipping plane. This defines the maximum Z-coordinate visible in the orthographic projection.
	[[nodiscard]] Matrix CreateOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

	// Represents a transformation
	struct Transform {
		Vector3f Translation = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f Rotation = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f Scale = Vector3f(1.0f, 1.0f, 1.0f);

		Matrix CreateTransformMatrix(Vector3f translationOffset = {0,0,0}) const {
			return CreateTranslationMatrix(Translation + translationOffset) *
				CreateRotationMatrixYawPitchRoll(Rotation) *
				CreateScaleMatrix(Scale);
		}
	};

	// Lerp between two values
	// a - The start value
	// b - The end value
	// t - The interpolation value
	// Clamps t to 0 and 1
	inline float Lerp(float a, float b, float t) {
		return a + t * (b - a);
	}
	inline	Vector2f Lerp(Vector2f a, Vector2f b, float t) {
		return Vector2f(
			Lerp(a.x, b.x, t), 
			Lerp(a.y, b.y, t)
		);
	}
	inline Vector3f Lerp(Vector3f a, Vector3f b, float t) {
		return Vector3f(
			Lerp(a.x, b.x, t),
			Lerp(a.y, b.y, t),
			Lerp(a.z, b.z, t)
		);
	}
	inline Vector4f Lerp(Vector4f a, Vector4f b, float t) {
		return Vector4f(
			Lerp(a.x, b.x, t),
			Lerp(a.y, b.y, t),
			Lerp(a.z, b.z, t),
			Lerp(a.w, b.w, t)
		);
	}

	// Get distance between two points
	inline float GetDistanceBetween(float a, float b) {
		return std::abs(a - b);
	}
	inline float GetDistanceBetween(Vector2f a, Vector2f b) {
		return glm::distance(a, b);
	}
	inline float GetDistanceBetween(Vector3f a, Vector3f b) {
		return glm::distance(a, b);
	}
	inline float GetDistanceBetween(Vector4f a, Vector4f b) {
		return glm::distance(a, b);
	}
}
