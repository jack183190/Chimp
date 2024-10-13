#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

layout(std140) uniform Camera {
    mat4 viewProjection;
};

layout (std140) uniform Model {
	mat4 model;
};

out vec2 v_TexCoords;

void main()
{
	mat4 mat = viewProjection * model;
    gl_Position = mat * vec4(position, 1.0);

	v_TexCoords = texCoords;
}
