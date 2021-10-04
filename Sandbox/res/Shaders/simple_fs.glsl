#version 400 core

struct Material
{
	vec3 ambient;
	sampler2D ambientTexture;

	vec3 diffuse;
	sampler2D diffuseTexture;

	vec3 specular;
	sampler2D specularTexture;

	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 outColor;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPosition;

uniform Material material;
uniform Light u_Light;

uniform vec3 u_CameraPosition;

float GetLightAttenuation(Light light, vec3 position)
{
	float distance = length(light.position - position);
	vec3 K = vec3(light.constant, light.linear, light.quadratic);
	vec3 d = vec3(1.0, distance, pow(distance, 2.0));
	return 1.0 / dot(K,d);
}

void main()
{

	// ambient
	vec3 ambient = u_Light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightDirection = normalize(u_Light.position - v_WorldPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = u_Light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDirection = normalize(u_CameraPosition - v_WorldPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specularFactor = dot(viewDirection, reflectDirection);
	float spec = specularFactor > 0 ? pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) : 0;
	vec3 specular = u_Light.specular * (spec * material.specular);


	float attenuation = GetLightAttenuation(u_Light, v_WorldPosition);
	vec3 result = (ambient + diffuse + specular);
	outColor = vec4(result, 1) * 
		texture(material.specularTexture, v_TexCoord) * 
		texture(material.diffuseTexture, v_TexCoord) * 
		texture(material.ambientTexture, v_TexCoord) * attenuation;
	
	
}