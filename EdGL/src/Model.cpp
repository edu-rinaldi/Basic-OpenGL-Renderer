#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

namespace edgl {

Model::Model(const char* path) : m_Position(glm::vec3(0.0f)), m_ModelMatrix(glm::mat4(1.0f))
{
	LoadModel(path);
}

Model::~Model()
{
}

glm::mat4 Model::GetModelMatrix() const
{
	return m_ModelMatrix;
}

void Model::Move(const glm::vec3& amount)
{
	// m_Position = m_Position + amount;
	m_ModelMatrix = glm::translate(m_ModelMatrix, amount);
}

void Model::Rotate(float amount, const glm::vec3& rotationAxes)
{
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(amount), rotationAxes);
}

void Model::Scale(const glm::vec3& amount)
{
	m_ModelMatrix = glm::scale(m_ModelMatrix, amount);
}

void Model::Draw(Shader& shader) const
{
	shader.Bind();
	shader.SetMatrix4f("u_Model", GetModelMatrix());
	for (const auto& mesh : m_Meshes)
		mesh->Draw(shader);
}

void Model::ApplyMaterial(std::shared_ptr<Material> material)
{
	for (auto& mesh : m_Meshes)
		mesh->SetMaterial(material);
}

void Model::LoadModel(const char* path)
{
	Assimp::Importer importer;
	// aiProcess_GenNormals ?
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	std::string pathStr(path);

	m_Directory = pathStr.substr(0, pathStr.find_last_of('/') + 1);

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::shared_ptr<Material> material;
	// Vertices
	vertices.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		// process vertex positions, normals and texture coordinates
		Vertex vertex;
		glm::vec3 position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		glm::vec3 normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		glm::vec2 texCoord = glm::vec2(0.f);
		if (mesh->mTextureCoords[0])
			texCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

		vertices.emplace_back(position, normal, texCoord);
	}

	// Indices
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	// Material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D color;
		aiString str;
		float shininess;

		aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_AMBIENT, &color);
		glm::vec3 ambientColor = { color.r, color.g, color.b };
		aiMaterial->GetTexture(aiTextureType_AMBIENT, 0, &str);
		std::shared_ptr<Texture> ambientTexture = strlen(str.C_Str()) == 0 ?
			Texture2D::FlatColor(glm::vec4(1.f), 1, 1, "white") :
			Texture2D::FromFile(std::string(m_Directory) + str.C_Str());


		aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &color);
		glm::vec3 diffuseColor = { color.r, color.g, color.b };
		aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		std::shared_ptr<Texture> diffuseTexture = strlen(str.C_Str()) == 0 ?
			Texture2D::FlatColor(glm::vec4(1.f), 1, 1, "white") :
			Texture2D::FromFile(std::string(m_Directory) + str.C_Str());


		aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_SPECULAR, &color);
		glm::vec3 specularColor = { color.r, color.g, color.b };
		aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &str);
		std::shared_ptr<Texture> specularTexture = strlen(str.C_Str()) == 0 ?
			Texture2D::FlatColor(glm::vec4(1.f), 1, 1, "white") :
			Texture2D::FromFile(std::string(m_Directory) + str.C_Str());

		aiMaterial->Get(AI_MATKEY_SHININESS, shininess);
		material = std::shared_ptr<Material>(
			new Material(ambientColor, ambientTexture,
				diffuseColor, diffuseTexture,
				specularColor, specularTexture,
				shininess));
	}
	else material = std::shared_ptr<Material>(new Material());
	return std::shared_ptr<Mesh>(new Mesh(vertices, indices, material));
}

}