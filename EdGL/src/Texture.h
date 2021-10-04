#pragma once
#include <GL/glew.h>
#include <memory>
#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace edgl {

enum TextureType
{
	TEXTURE_1D = GL_TEXTURE_1D,
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_3D = GL_TEXTURE_3D,
	TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
	TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
	TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
	TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
	TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
	TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
	TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
	TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

class Texture
{
public:
	// Getters
	virtual inline unsigned int GetId() const { return m_Id; }
	virtual inline const std::unique_ptr<uint8_t>& GetTextureData() const { return m_Data; }

	// Setters
	// Texture Bind() / Unbind()
	virtual void Bind(int unit) const;
	virtual void Unbind() const;

protected:
	unsigned int m_Id;
	std::unique_ptr<uint8_t> m_Data;
	TextureType m_Type;
	std::string m_CacheName;

	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;

	Texture(TextureType targetType, const std::string& textureName);
	~Texture();
private:

	Texture() = delete;
	Texture(const Texture& obj) = delete;
};

class Texture2D : public Texture
{
public:
	static std::shared_ptr<Texture> FlatColor(const glm::vec3& color, int width, int height, const std::string& textureName);
	static std::shared_ptr<Texture> FlatColor(const glm::vec4& color, int width, int height, const std::string& textureName);
	static std::shared_ptr<Texture> FromFile(const std::string& texturePath, const std::string& textureName);
	static std::shared_ptr<Texture> FromFile(const std::string& texturePath);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetNumberOfChannels() const { return m_NrChannels; }
private:
	Texture2D(const std::string& texturePath, const std::string& textureName);
	Texture2D(const glm::vec3& color, int width, int height, const std::string& textureName);
	Texture2D(const glm::vec4& color, int width, int height, const std::string& textureName);

	int m_Width;
	int m_Height;
	int m_NrChannels;
};

class FailedToLoadTextureException : public std::exception
{
public:
	FailedToLoadTextureException(const std::string& texturePath) :
		m_Message(std::string("Failed to load texture located at this path: ") + texturePath) {}
private:
	std::string m_Message;
	inline virtual const char* what() const throw() override
	{
		return m_Message.c_str();
	}
};

class FailedToCreateTexture : public std::exception
{
	inline virtual const char* what() const throw() override { return "Failed to create texture"; }
};

}