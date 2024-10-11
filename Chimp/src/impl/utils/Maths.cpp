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

	Matrix CreateViewMatrix(Vector3f position, Vector3f target, Vector3f up)
	{
		return glm::lookAt(position, target, up);
	}

	Matrix CreateOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}
}
