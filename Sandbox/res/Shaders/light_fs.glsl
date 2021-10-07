#version 400 core

out vec4 o_FragColor;

uniform vec3 u_LightColor;

void main()
{
    o_FragColor = vec4(u_LightColor, 1);
}