#version 330 core
layout (location = 0) in vec3 aPos;

layout(std140) uniform Camera {
    mat4 viewProjection;
};

void main()
{
    gl_Position = viewProjection * vec4(aPos, 1.0);
}
