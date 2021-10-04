#include "VertexArray.h"

namespace edgl {

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
	glBindVertexArray(m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	glBindVertexArray(m_Id);
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < layout.GetElements().size(); ++i)
	{
		const VertexBufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * element.GetSize();
	}
}

void VertexArray::AddIndexBuffer(const IndexBuffer& ib)
{
	glBindVertexArray(m_Id);
	ib.Bind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

}