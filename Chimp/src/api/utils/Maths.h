#pragma once

#include <vector>
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
	[[nodiscard]] Vector2f VectorNormalized(const Vector2f& a);

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

		Transform(Vector3f translation = { 0,0,0 }, Vector3f rotation = { 0,0,0 }, Vector3f scale = { 1,1,1 }) :
			Translation{ translation }, Rotation{ rotation }, Scale{ scale } {
		}
		Transform(Vector2f translation) :
			Transform({ translation.x, translation.y, 0 }) {
		}

		Matrix CreateTransformMatrix(Vector3f translationOffset = { 0,0,0 }) const {
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

	// Get squared distance between two points
	inline float GetSquaredDistanceBetween(float a, float b) {
		return (a - b) * (a - b);
	}
	inline float GetSquaredDistanceBetween(Vector2f a, Vector2f b) {
		const auto diff = a - b;
		return glm::dot(diff, diff);
	}
	inline float GetSquaredDistanceBetween(Vector3f a, Vector3f b) {
		const auto diff = a - b;
		return glm::dot(diff, diff);
	}
	inline float GetSquaredDistanceBetween(Vector4f a, Vector4f b) {
		const auto diff = a - b;
		return glm::dot(diff, diff);
	}

	// Get length of a vector
	inline float Length(float a) {
		return std::abs(a);
	}
	inline float Length(Vector2f a) {
		return glm::length(a);
	}
	inline float Length(Vector3f a) {
		return glm::length(a);
	}
	inline float Length(Vector4f a) {
		return glm::length(a);
	}

	// Squared length of a vector
	inline float SquaredLength(float a) {
		return a * a;
	}
	inline float SquaredLength(Vector2f a) {
		return glm::dot(a, a);
	}
	inline float SquaredLength(Vector3f a) {
		return glm::dot(a, a);
	}
	inline float SquaredLength(Vector4f a) {
		return glm::dot(a, a);
	}

	// Returns minimum components of two values (e.g (2,3) and (1,4) would return (1,3))
	inline float ComponentMin(float a, float b) {
		return std::min(a, b);
	}
	inline Vector2f ComponentMin(Vector2f a, Vector2f b) {
		return Vector2f(
			ComponentMin(a.x, b.x),
			ComponentMin(a.y, b.y)
		);
	}
	inline Vector3f ComponentMin(Vector3f a, Vector3f b) {
		return Vector3f(
			ComponentMin(a.x, b.x),
			ComponentMin(a.y, b.y),
			ComponentMin(a.z, b.z)
		);
	}
	inline Vector4f ComponentMin(Vector4f a, Vector4f b) {
		return Vector4f(
			ComponentMin(a.x, b.x),
			ComponentMin(a.y, b.y),
			ComponentMin(a.z, b.z),
			ComponentMin(a.w, b.w)
		);
	}

	// Returns maximum components of two values
	inline float ComponentMax(float a, float b) {
		return std::max(a, b);
	}
	inline Vector2f ComponentMax(Vector2f a, Vector2f b) {
		return Vector2f(
			ComponentMax(a.x, b.x),
			ComponentMax(a.y, b.y)
		);
	}
	inline Vector3f ComponentMax(Vector3f a, Vector3f b) {
		return Vector3f(
			ComponentMax(a.x, b.x),
			ComponentMax(a.y, b.y),
			ComponentMax(a.z, b.z)
		);
	}
	inline Vector4f ComponentMax(Vector4f a, Vector4f b) {
		return Vector4f(
			ComponentMax(a.x, b.x),
			ComponentMax(a.y, b.y),
			ComponentMax(a.z, b.z),
			ComponentMax(a.w, b.w)
		);
	}

	// Multiply components of two values
	// e.g (2,3) and (1,4) would return (2,12)
	inline float ComponentMultiply(float a, float b) {
		return a * b;
	}
	inline Vector2f ComponentMultiply(Vector2f a, Vector2f b) {
		return Vector2f(
			ComponentMultiply(a.x, b.x),
			ComponentMultiply(a.y, b.y)
		);
	}
	inline Vector3f ComponentMultiply(Vector3f a, Vector3f b) {
		return Vector3f(
			ComponentMultiply(a.x, b.x),
			ComponentMultiply(a.y, b.y),
			ComponentMultiply(a.z, b.z)
		);
	}
	inline Vector4f ComponentMultiply(Vector4f a, Vector4f b) {
		return Vector4f(
			ComponentMultiply(a.x, b.x),
			ComponentMultiply(a.y, b.y),
			ComponentMultiply(a.z, b.z),
			ComponentMultiply(a.w, b.w)
		);
	}

	// Clamp components of a value between a minimum and maximum
	inline float ComponentClamp(float value, float min, float max) {
		return value < min ? min : value > max ? max : value;
	}
	inline Vector2f ComponentClamp(Vector2f value, Vector2f min, Vector2f max) {
		return Vector2f(
			ComponentClamp(value.x, min.x, max.x),
			ComponentClamp(value.y, min.y, max.y)
		);
	}
	inline Vector3f ComponentClamp(Vector3f value, Vector3f min, Vector3f max) {
		return Vector3f(
			ComponentClamp(value.x, min.x, max.x),
			ComponentClamp(value.y, min.y, max.y),
			ComponentClamp(value.z, min.z, max.z)
		);
	}
	inline Vector4f ComponentClamp(Vector4f value, Vector4f min, Vector4f max) {
		return Vector4f(
			ComponentClamp(value.x, min.x, max.x),
			ComponentClamp(value.y, min.y, max.y),
			ComponentClamp(value.z, min.z, max.z),
			ComponentClamp(value.w, min.w, max.w)
		);
	}

	// Find closest position to a target
	// target - The target position
	// positions - The positions to compare, cannot be empty
	// Returns the index of the closest position
	[[nodiscard]] inline size_t FindClosest(Vector2f target, const std::vector<Vector2f>& positions) {
		assert(!positions.empty());
		size_t closestIndex = 0;
		float closestDistance = GetSquaredDistanceBetween(target, positions[closestIndex]);
		for (size_t i = closestIndex + 1; i < positions.size(); ++i) {
			float distance = GetSquaredDistanceBetween(target, positions[i]);
			if (distance < closestDistance) {
				closestIndex = i;
				closestDistance = distance;
			}
		}
		return closestIndex;
	}

	// Arc tangent of two values
	// returns angle in radians
	[[nodiscard]] inline float Atan2(float y, float x) {
		return glm::atan(y, x);
	}
	[[nodiscard]] inline float Atan2(Vector2f direction) {
		return glm::atan(direction.y, direction.x);
	}

	// Dot product
	[[nodiscard]] inline float Dot(Vector2f a, Vector2f b) {
		return glm::dot(a, b);
	}
	[[nodiscard]] inline float Dot(Vector3f a, Vector3f b) {
		return glm::dot(a, b);
	}
	[[nodiscard]] inline float Dot(Vector4f a, Vector4f b) {
		return glm::dot(a, b);
	}
}
