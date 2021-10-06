#pragma once
#include <glm/glm.hpp>

namespace edgl {

struct Vertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_TexCoord;
	glm::vec3 m_Tangent;

	Vertex() : 
		m_Position(glm::vec3(0.f)), 
		m_Normal(glm::vec3(0.f)), 
		m_TexCoord(glm::vec3(0.f)), 
		m_Tangent(glm::vec3(0.f)) 
	{
	}
	
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoord, const glm::vec3& tangent) :
		m_Position(position), 
		m_Normal(normal), 
		m_TexCoord(texCoord), 
		m_Tangent(tangent) 
	{
	}
};

}