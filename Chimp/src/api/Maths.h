#pragma once

// Done so glm can be swapped out by just changing this file rather than the whole project

#include <../vendor/glm/glm/glm.hpp>

namespace Chimp {
	typedef glm::vec2 Vector2f;
	typedef glm::vec3 Vector3f;
	typedef glm::vec4 Vector4f;
	typedef glm::mat4x4 Matrix;
}