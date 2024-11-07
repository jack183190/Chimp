#version 330 core

in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;

void main()
{
    FragColor = texture(u_ActiveTexture, v_TexCoords);
    if (FragColor.a < 1) // A pixel with 0 alpha may not have 0 alpha here, either due to anti aliasing, frag not lining up with pixel, or some other reason
	{
		discard;
	}
}
