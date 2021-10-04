#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

namespace edgl {

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Material;

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::shared_ptr<Material> material);
	~Mesh();
	inline const VertexArray& GetVAO() const { return *m_Vao; }
	inline const VertexBuffer& GetVBO() const { return *m_Vbo; }
	inline const IndexBuffer& GetIBO() const { return *m_Ibo; }
	inline unsigned int GetVerticesNumber() const { return m_VerticesNumber; }
	inline unsigned int GetIndicesNumber() const { return m_IndicesNumber; }

	void Draw(Shader& shader) const;
	inline void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

private:
	unsigned int m_VerticesNumber;
	unsigned int m_IndicesNumber;

	std::shared_ptr<Material> m_Material;

	VertexArray* m_Vao;
	VertexBuffer* m_Vbo;
	IndexBuffer* m_Ibo;

};

}
