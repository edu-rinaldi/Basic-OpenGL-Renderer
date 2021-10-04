#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace edgl {

class VertexBuffer
{
private:
	unsigned int m_Id;
	unsigned int m_Size;
public:
	VertexBuffer(void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

}