#version 330 core

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;

void main()
{
    FragColor = vec4(texture(u_ActiveTexture, vec2(0.5f, 0.5f)).xyz, 1.0f);
}
