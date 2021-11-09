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

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 outColor;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPosition;

uniform Material material;

#define MAX_POINT_LIGHTS 128
#define MAX_SPOT_LIGHTS 128

uniform DirectionalLight u_DirectionalLight;

uniform PointLight	u_PointLights[MAX_POINT_LIGHTS];
uniform int u_NrOfPointLights;

uniform SpotLight 	u_SpotLights[MAX_SPOT_LIGHTS];
uniform int u_NrOfSpotLights;

uniform vec3 u_CameraPosition;

float GetLightAttenuation(PointLight light, vec3 position);
vec4 CalcLight(vec3 lightDiffuse, vec3 lightSpecular, Material material, vec3 normal, vec3 viewDirection, vec3 lightDirection);
vec4 CalcDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewDirection);
vec4 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec4 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{

	// ambient
	vec4 result = vec4(0);
	
	vec3 viewDirection = normalize(u_CameraPosition - v_WorldPosition);
	// diffuse
	vec3 norm = normalize(v_Normal);

	result = vec4(u_DirectionalLight.ambient * material.ambient, 1);
	result += CalcDirectionalLight(u_DirectionalLight, material, norm, viewDirection);
	
	for(int i=0; i < u_NrOfPointLights; ++i)
		result += CalcPointLight(u_PointLights[i], material, norm, v_WorldPosition, viewDirection);
	
	// for(int i=0; i < u_NrOfSpotLights; ++i)
	// 	result += CalcSpotLight(u_SpotLights[i], material, norm, v_WorldPosition, viewDirection);
	
	outColor = result;
	
}

float GetLightAttenuation(PointLight light, vec3 position)
{
	float distance = length(light.position - position);
	vec3 K = vec3(light.constant, light.linear, light.quadratic);
	vec3 d = vec3(1.0, distance, pow(distance, 2.0));
	return 1.0 / dot(K,d);
}

vec4 CalcLight(vec3 lightDiffuse, vec3 lightSpecular, Material material, vec3 normal, vec3 viewDirection, vec3 lightDirection)
{
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = lightDiffuse * (diff * material.diffuse * texture(material.diffuseTexture, v_TexCoord).xyz);

	// specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularFactor = dot(viewDirection, reflectDirection);
	float spec = specularFactor > 0 ? pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) : 0;
	vec3 specular = lightSpecular * (spec * material.specular * texture(material.specularTexture, v_TexCoord).xyz);

	// combine results
	vec4 result = vec4(specular + diffuse, 1);
	return result;
}

vec4 CalcDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);
	return CalcLight(light.diffuse, light.specular, material, normal, viewDirection, lightDirection);
}

vec4 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragPos);
	return CalcLight(light.diffuse, light.specular, material, normal, viewDirection, lightDirection) * GetLightAttenuation(light, fragPos);
}

vec4 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragPos);
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);
	// compute light
	return CalcLight(light.diffuse, light.specular, material, normal, viewDirection, lightDirection) * intensity;
}