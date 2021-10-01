#pragma once
#include <iostream>
#include <GL/glew.h>
#include <memory>
#include <unordered_map>

class Texture
{
public:
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

private:
	unsigned int m_Id;
	unsigned char* m_Texture;
	int m_Width;
	int m_Height;
	int m_NrChannels;
	enum TextureType m_Type;
	// TODO: Implement texture caching
	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;
	std::string m_CacheName;
	
	Texture();
	Texture(const Texture& obj);
	Texture(unsigned char* texture, int width, int height, int channels, const std::string& name, TextureType targetType = TEXTURE_2D);
public:

	// Texture(const Texture& obj);
	// Texture();
	// Texture(const std::string& texturePath, TextureType targetType = TEXTURE_2D);
	~Texture();
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& texturePath, TextureType targetType = TEXTURE_2D);
	static std::shared_ptr<Texture> BlankTexture();
	// Getters
	inline unsigned int GetId() const { return m_Id; }
	inline const unsigned char* GetTextureData() const { return m_Texture; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetNumberOfChannels() const { return m_NrChannels; }

	// Setters

	// Texture Bind() / Unbind()
	void Bind(int unit) const;
	void Unbind() const;
};