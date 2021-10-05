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
	vec3 direction;
	float cutOff;
	float outCutOff;

	bool hasPosition;
	bool hasDirection;

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
uniform Light u_Light[10];

uniform vec3 u_CameraPosition;

float GetLightAttenuation(Light light, vec3 position);
vec4 CalcLight(Light light, Material material, vec3 normal, vec3 viewDirection, vec3 lightDirection);
vec4 CalcDirectionalLight(Light light, Material material, vec3 normal, vec3 viewDirection);
vec4 CalcPointLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec4 CalcSpotLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{

	// ambient
	vec4 result = vec4(0);
	
	vec3 viewDirection = normalize(u_CameraPosition - v_WorldPosition);
	// diffuse
	vec3 norm = normalize(v_Normal);

	result = vec4(u_Light[0].ambient * material.ambient, 1);
	result += CalcDirectionalLight(u_Light[0], material, norm, viewDirection);
	
	for(int i=1; i < 6; i++)
		result += CalcPointLight(u_Light[i], material, norm, v_WorldPosition, viewDirection);
	
	for(int i=6; i < 10; i++)
		result += CalcSpotLight(u_Light[i], material, norm, v_WorldPosition, viewDirection);
	
	
	outColor = result;
	// vec3 lightDirection = normalize(u_Light.position - v_WorldPosition);
	// float diff = max(dot(norm, lightDirection), 0.0);
	// vec3 diffuse = u_Light.diffuse * (diff * material.diffuse);

	// // specular
	// vec3 viewDirection = normalize(u_CameraPosition - v_WorldPosition);
	// vec3 reflectDirection = reflect(-lightDirection, norm);
	// float specularFactor = dot(viewDirection, reflectDirection);
	// float spec = specularFactor > 0 ? pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) : 0;
	// vec3 specular = u_Light.specular * (spec * material.specular);

	// float attenuation = GetLightAttenuation(u_Light, v_WorldPosition);
	// vec3 result = (ambient + diffuse + specular) ;
	// outColor = vec4(result, 1) * 
	// 	texture(material.specularTexture, v_TexCoord) * 
	// 	texture(material.diffuseTexture, v_TexCoord) * 
	// 	texture(material.ambientTexture, v_TexCoord) * attenuation;
	
	
}

float GetLightAttenuation(Light light, vec3 position)
{
	float distance = length(light.position - position);
	vec3 K = vec3(light.constant, light.linear, light.quadratic);
	vec3 d = vec3(1.0, distance, pow(distance, 2.0));
	return 1.0 / dot(K,d);
}

vec4 CalcLight(Light light, Material material, vec3 normal, vec3 viewDirection, vec3 lightDirection)
{
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularFactor = dot(viewDirection, reflectDirection);
	float spec = specularFactor > 0 ? pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) : 0;
	vec3 specular = light.specular * (spec * material.specular);

	// combine results
	vec4 result = vec4(specular + diffuse, 1);
	return result * texture(material.specularTexture, v_TexCoord) * texture(material.diffuseTexture, v_TexCoord);
}

vec4 CalcDirectionalLight(Light light, Material material, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);
	return CalcLight(light, material, normal, viewDirection, lightDirection);
}

vec4 CalcPointLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragPos);
	return CalcLight(light, material, normal, viewDirection, lightDirection) * GetLightAttenuation(light, fragPos);
}

vec4 CalcSpotLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragPos);
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);
	// compute light
	return CalcLight(light, material, normal, viewDirection, lightDirection) * intensity;
}