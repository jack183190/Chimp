#version 330 core

in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;

void main()
{
    FragColor = vec4(texture(u_ActiveTexture, v_TexCoords).xyz, 1.0f);
}
