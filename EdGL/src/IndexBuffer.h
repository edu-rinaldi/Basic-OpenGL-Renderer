#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
private:
	unsigned int m_Id;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};