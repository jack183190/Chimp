#version 330 core

in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;

void main()
{
    FragColor = texture(u_ActiveTexture, -2.0f * v_TexCoords);
}
