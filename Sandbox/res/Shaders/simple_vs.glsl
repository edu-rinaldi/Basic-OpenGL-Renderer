#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec2 tangent;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform mat4 u_TransposedInverseModel;


out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPosition;

void main()
{
	gl_Position		= u_Projection * u_View * u_Model * vec4(position, 1);
	v_Normal = normalize((u_TransposedInverseModel * vec4(normal, 1)).xyz);
	v_TexCoord		= texCoord;
	v_WorldPosition = (u_Model * vec4(position, 1)).xyz;
}