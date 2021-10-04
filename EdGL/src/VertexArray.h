#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

namespace edgl {

class VertexArray
{
private:
	unsigned int m_Id;
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddIndexBuffer(const IndexBuffer& ib);

	void Bind() const;
	void Unbind() const;
};

}
