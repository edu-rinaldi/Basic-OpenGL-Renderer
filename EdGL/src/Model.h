#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace edgl {

class Material;
class Mesh;
class Shader;

class Model
{
public:
	Model(const char* path);
	~Model();
	inline const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
	inline const glm::vec3 GetPosition() const { return GetModelMatrix() * glm::vec4(m_Position, 1); }

	glm::mat4 GetModelMatrix() const;
	void Move(const glm::vec3& amount);
	void Rotate(float amount, const glm::vec3& rotationAxes);
	void Scale(const glm::vec3& amount);
	void Draw(Shader& shader) const;
	void ApplyMaterial(std::shared_ptr<Material> material);
	void EnableCullFace(bool value);
private:
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
	glm::vec3 m_Position;
	glm::mat4 m_ModelMatrix;
	std::string m_Directory;

	void LoadModel(const char* path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
}