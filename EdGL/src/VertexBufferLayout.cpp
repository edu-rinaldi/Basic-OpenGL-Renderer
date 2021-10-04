#include "VertexBufferLayout.h"

namespace edgl {

VertexBufferLayout::VertexBufferLayout() : m_Elements(std::vector<VertexBufferElement>()), m_Stride(0)
{
}

void VertexBufferLayout::Push(VertexBufferElement element)
{
	m_Elements.push_back(element);
	m_Stride += element.count * element.GetSize();
}

}
