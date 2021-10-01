#pragma once

#include<glm/glm.hpp>
#include"Texture.h"

class Material
{
private:
	glm::vec3 m_Ambient;
	std::shared_ptr<Texture> m_AmbientTexture;

	glm::vec3 m_Diffuse;
	std::shared_ptr<Texture> m_DiffuseTexture;

	glm::vec3 m_Specular;
	std::shared_ptr<Texture> m_SpecularTexture;
	
	float m_Shininess;

public:
	Material();
	Material(
		const glm::vec3& ambient, std::shared_ptr<Texture> ambientTexture,
		const glm::vec3& diffuse, std::shared_ptr<Texture> diffuseTexture,
		const glm::vec3& specular, std::shared_ptr<Texture> specularTexture,
		float shininess);
	~Material();

	// Getters
	inline glm::vec3 GetAmbientColor() const { return m_Ambient; }
	inline std::shared_ptr<Texture> GetAmbientTexture() const { return m_AmbientTexture; }

	inline glm::vec3 GetDiffuseColor() const { return m_Diffuse; }
	inline std::shared_ptr<Texture> GetDiffuseTexture() const { return m_DiffuseTexture; }

	inline glm::vec3 GetSpecularColor() const { return m_Specular; }
	inline std::shared_ptr<Texture> GetSpecularTexture() const { return m_SpecularTexture; }

	inline float GetShininess() const { return m_Shininess; }
};

