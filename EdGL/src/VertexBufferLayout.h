#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned int normalized;

	inline unsigned int GetSize() const
	{
		switch(type) 
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
			default: throw std::invalid_argument("Not valid VertexBufferElement::type");
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout();
	void Push(VertexBufferElement element);

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
