#include "Mesh.h"
#include "VertexArray.h"
#include "Material.h"

namespace edgl {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Ref<Material> material) :
	m_VerticesNumber(vertices.size()),
	m_IndicesNumber(indices.size()),
	m_Material(material),
	m_ApplyCullFace(true),
	m_Vao(new VertexArray()),
	m_Vbo(new VertexBuffer((void*)vertices.data(), vertices.size() * sizeof(Vertex))),
	m_Ibo(new IndexBuffer(indices.data(), indices.size()))
{
	VertexBufferLayout vbl;
	vbl.Push({ 3, GL_FLOAT, GL_FALSE });
	vbl.Push({ 3, GL_FLOAT, GL_FALSE });
	vbl.Push({ 2, GL_FLOAT, GL_FALSE });
	vbl.Push({ 3, GL_FLOAT, GL_FALSE });

	m_Vao->AddVertexBuffer(*m_Vbo, vbl);
	m_Vao->AddIndexBuffer(*m_Ibo);
}

Mesh::~Mesh()
{
	// Free memory
	delete m_Vao;
	delete m_Vbo;
	delete m_Ibo;
}

void Mesh::Draw(Shader& shader) const
{
	int numberOfTexture = 0;

	shader.Bind();

	// Ambient color and texture
	shader.SetVec3("material.ambient", m_Material->GetAmbientColor());
	m_Material->GetAmbientTexture()->Bind(numberOfTexture);
	shader.SetInt("material.ambientTexture", numberOfTexture++);

	// Diffuse color and texture
	shader.SetVec3("material.diffuse", m_Material->GetDiffuseColor());
	m_Material->GetDiffuseTexture()->Bind(numberOfTexture);
	shader.SetInt("material.diffuseTexture", numberOfTexture++);

	// Specular color and texture
	shader.SetVec3("material.specular", m_Material->GetSpecularColor());
	m_Material->GetSpecularTexture()->Bind(numberOfTexture);
	shader.SetInt("material.specularTexture", numberOfTexture++);

	shader.SetFloat("material.shininess", m_Material->GetShininess());

	m_Vao->Bind();
	bool cullface = CanApplyCullFace();
	bool flag = false;
	if (!cullface && glIsEnabled(GL_CULL_FACE)) 
	{
		glDisable(GL_CULL_FACE); 
		flag = true;
	}
	glDrawElements(GL_TRIANGLES, m_IndicesNumber, GL_UNSIGNED_INT, 0);
	if (flag) glEnable(GL_CULL_FACE);
	m_Vao->Unbind();
}

}


