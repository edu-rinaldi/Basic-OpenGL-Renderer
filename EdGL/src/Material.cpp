#include "Material.h"

namespace edgl {

Material::Material() :
	m_Ambient(glm::vec3(0.9f)),
	m_AmbientTexture(),
	m_Diffuse(glm::vec3(1.f)),
	m_DiffuseTexture(),
	m_Specular(glm::vec3(1.f)),
	m_SpecularTexture(),
	m_Shininess(32)
{

}

Material::Material(
	const glm::vec3& ambient, std::shared_ptr<Texture> ambientTexture,
	const glm::vec3& diffuse, std::shared_ptr<Texture> diffuseTexture,
	const glm::vec3& specular, std::shared_ptr<Texture> specularTexture,
	float shininess)
	: m_Ambient(ambient), m_AmbientTexture(ambientTexture),
	m_Diffuse(diffuse), m_DiffuseTexture(diffuseTexture),
	m_Specular(specular), m_SpecularTexture(specularTexture),
	m_Shininess(shininess)
{
}

Material::~Material()
{
}

}