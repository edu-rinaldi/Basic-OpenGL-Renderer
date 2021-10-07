#version 400 core

layout (location = 0) in vec3 pos;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

void main()
{
	gl_Position =  u_Projection * u_View * u_Model * vec4(pos, 1.0);
}