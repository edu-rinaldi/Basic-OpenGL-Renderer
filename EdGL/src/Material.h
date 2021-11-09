#pragma once

#include<glm/glm.hpp>
#include"Texture.h"

namespace edgl {

class Material
{
private:
	glm::vec3 m_Ambient;
	Ref<Texture> m_AmbientTexture;

	glm::vec3 m_Diffuse;
	Ref<Texture> m_DiffuseTexture;

	glm::vec3 m_Specular;
	Ref<Texture> m_SpecularTexture;

	float m_Shininess;

public:
	Material();
	Material(
		const glm::vec3& ambient, Ref<Texture> ambientTexture,
		const glm::vec3& diffuse, Ref<Texture> diffuseTexture,
		const glm::vec3& specular, Ref<Texture> specularTexture,
		float shininess);
	~Material();

	// Getters
	inline glm::vec3 GetAmbientColor() const { return m_Ambient; }
	inline Ref<Texture> GetAmbientTexture() const { return m_AmbientTexture; }

	inline glm::vec3 GetDiffuseColor() const { return m_Diffuse; }
	inline Ref<Texture> GetDiffuseTexture() const { return m_DiffuseTexture; }

	inline glm::vec3 GetSpecularColor() const { return m_Specular; }
	inline Ref<Texture> GetSpecularTexture() const { return m_SpecularTexture; }

	inline float GetShininess() const { return m_Shininess; }
};

}