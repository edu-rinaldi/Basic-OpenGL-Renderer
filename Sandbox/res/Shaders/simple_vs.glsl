#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;


out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPosition;

void main()
{
	gl_Position		= u_Projection * u_View * u_Model * vec4(position, 1);
	// v_Normal		= (u_Model * vec4(normal, 0)).xyz;
	// mat3(transpose(inverse(view * model))) * aNormal
	v_Normal = mat3(transpose(inverse(u_Model))) * normal;
	
	v_TexCoord		= texCoord;
	// v_WorldPosition = mat3(transpose(inverse(u_Model))) * position;
	v_WorldPosition = (u_Model * vec4(position, 1)).xyz;
};