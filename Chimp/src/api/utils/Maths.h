#pragma once

#include <../vendor/glm/glm/glm.hpp>
#include <../vendor/glm/glm/gtc/matrix_transform.hpp>
#include <../vendor/glm/glm/gtc/type_ptr.hpp>

// Done so glm can be swapped out by just changing this file rather than the whole project

namespace Chimp {
	typedef glm::vec2 Vector2f;
	typedef glm::vec3 Vector3f;
	typedef glm::vec4 Vector4f;

	typedef glm::vec<2, int> Vector2i;
	typedef glm::vec<3, int> Vector3i;
	typedef glm::vec<4, int> Vector4i;

	typedef glm::mat4x4 Matrix;

	// Cross product of two vectors
	[[nodiscard]] Vector3f VectorCrossProduct(const Vector3f& a, const Vector3f& b);

	// Normalize a vector
	[[nodiscard]] Vector3f VectorNormalized(const Vector3f& a);

	// Create an identity matrix
	[[nodiscard]] Matrix CreateIdentityMatrix();

	// Create a translation matrix
	// translation - The translation vector
	[[nodiscard]] Matrix CreateTranslationMatrix(Vector3f translation);

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
}
