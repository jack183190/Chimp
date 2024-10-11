#version 330 core
layout (location = 0) in vec3 aPos;

layout(std140) uniform Camera {
    mat4 viewProjection;
};

layout (std140) uniform Model {
	mat4 model;
};

void main()
{
	mat4 mat = viewProjection * model;
    gl_Position = mat * vec4(aPos, 1.0);
}
