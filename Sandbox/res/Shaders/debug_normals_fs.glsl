#version 330 core

out vec4 outColor;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPosition;

void main()
{
	outColor = vec4(v_Normal * 0.5 + vec3(0.5), 1);
}