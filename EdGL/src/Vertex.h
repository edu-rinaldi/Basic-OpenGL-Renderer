#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_TexCoord;
	Vertex() : m_Position(glm::vec3(0.f)), m_Normal(glm::vec3(0.f)), m_TexCoord(glm::vec3(0.f)) {}
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoord) :
		m_Position(position), m_Normal(normal), m_TexCoord(texCoord) {}
};