#version 330 core

struct Material
{
	vec3 ambient;

	vec3 diffuse;


	vec3 specular;


	float shininess;
};

out vec4 outColor;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPosition;

Material material;

uniform vec3 u_CameraPosition;

void main()
{
	material.ambient = vec3(1);
	material.diffuse = vec3(1);
	material.specular = vec3(1);
	material.shininess = 32;

	vec3 lightColor = vec3(1);
	vec3 lightPosition = u_CameraPosition;
	vec3 ambientLightColor = vec3(0, 0.0, 0.0);
	// ambient
	vec3 ambient = ambientLightColor * material.ambient;

	// diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightDirection = normalize(lightPosition - v_WorldPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = lightColor * (diff * material.diffuse);

	// specular
	vec3 viewDirection = normalize(u_CameraPosition - v_WorldPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * material.specular) ;
	
	vec3 result = ambient + diffuse + specular;
	outColor = vec4(result, 1);
	
	
};