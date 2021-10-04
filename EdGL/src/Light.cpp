#include "Light.h"
#include "Shader.h"

namespace edgl {

Light::Light(const std::string& name, 
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
	float constant, float linear, float quadratic)	:
	m_Name(name),
	m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular),
	m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic),
	m_HasPosition(false), m_HasDirection(false)
{
}

void Light::AddToShader(Shader& shader) const
{
	shader.Bind();

	shader.SetVec3((m_Name + ".ambient").c_str(), m_Ambient);
	shader.SetVec3((m_Name + ".diffuse").c_str(), m_Diffuse);
	shader.SetVec3((m_Name + ".specular").c_str(), m_Specular);

	shader.SetFloat((m_Name + ".constant").c_str(), m_Constant);
	shader.SetFloat((m_Name + ".linear").c_str(), m_Linear);
	shader.SetFloat((m_Name + ".quadratic").c_str(), m_Quadratic);

	shader.SetBool((m_Name + ".hasDirection").c_str(), m_HasDirection);
	shader.SetBool((m_Name + ".hasPosition").c_str(), m_HasPosition);

	shader.Unbind();
}


// Directional light
DirectionalLight::DirectionalLight(const std::string& name, const glm::vec3& direction,
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
	float constant, float linear, float quadratic) :
	Light(name, ambient, diffuse, specular, constant, linear, quadratic),
	m_Direction(direction)
{
	m_HasDirection = true;
}

void DirectionalLight::AddToShader(Shader& shader) const
{
	Light::AddToShader(shader);
	shader.Bind();
	shader.SetVec3((m_Name + ".direction").c_str(), m_Direction);
	shader.Unbind();
}

// Point light


PointLight::PointLight(const std::string& name, const glm::vec3& position,
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
	float constant, float linear, float quadratic) :
		Light(name, ambient, diffuse, specular, constant, linear, quadratic),
		m_Position(position)
{
	m_HasPosition = true;
}

void PointLight::AddToShader(Shader& shader) const
{
	Light::AddToShader(shader);

	shader.Bind();
	shader.SetVec3((m_Name + ".position").c_str(), m_Position);
	shader.Unbind();
}

SpotLight::SpotLight(const std::string& name, const glm::vec3& position, const glm::vec3& direction, float cutOff,
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
	float constant, float linear, float quadratic) :
		Light(name, ambient, diffuse, specular, constant, linear, quadratic),
		m_Position(position), m_Direction(direction), m_CutOff(cutOff)
{
	m_HasDirection = true;
	m_HasPosition = true;
}

void SpotLight::AddToShader(Shader& shader) const
{
	Light::AddToShader(shader);
	shader.Bind();
	shader.SetVec3((m_Name + ".position").c_str(), m_Position);
	shader.SetVec3((m_Name + ".direction").c_str(), m_Direction);
	shader.Unbind();
}

}