#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::m_Cache = std::unordered_map<std::string, std::shared_ptr<Texture>>{};

Texture::Texture(const Texture& obj) : 
    m_Id(obj.m_Id),
    m_Width(obj.m_Width),
    m_Height(obj.m_Height),
    m_NrChannels(obj.m_NrChannels),
    m_Type(obj.m_Type)
{
    m_Texture = new unsigned char[obj.m_Height * obj.m_Width * obj.m_NrChannels];
    memcpy(m_Texture, obj.m_Texture, obj.m_Height * obj.m_Width * obj.m_NrChannels);
}

Texture::Texture() : Texture(new unsigned char [4] {255, 255, 255, 255}, 1, 1, 4, "GLBlankTexture")
{
}

Texture::Texture(unsigned char* texture, int width, int height, int channels, const std::string& name, TextureType targetType) : 
    m_Texture(texture), m_Width(width), m_Height(height), m_NrChannels(channels), m_Type(targetType), m_CacheName(name)
{   
    // Generating texture
    glGenTextures(1, &m_Id);
    glBindTexture(m_Type, m_Id);
    //glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(m_Type, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Texture);
    glGenerateMipmap(m_Type);
    
    glBindTexture(m_Type, 0);
}

std::shared_ptr<Texture> Texture::LoadTextureFromFile(const std::string& texturePath, TextureType targetType)
{
    if (m_Cache.find(texturePath) != m_Cache.end())
        return m_Cache[texturePath];
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureArray = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);
    
    if (textureArray == nullptr) return Texture::BlankTexture();

    std::shared_ptr<Texture> texture(new Texture(textureArray, width, height, channels, texturePath, targetType));
    m_Cache[texturePath] = texture;
    return texture;
}

std::shared_ptr<Texture> Texture::BlankTexture()
{
    std::string blankTextureName = "GLBlankTexture";
    if (m_Cache.find(blankTextureName) != m_Cache.end())
        return m_Cache[blankTextureName];

    std::shared_ptr<Texture> texture(new Texture());
    m_Cache[blankTextureName] = texture;
    return texture;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Id);
    stbi_image_free(m_Texture);
    
    // m_Cache.erase(m_CacheName);
}



void Texture::Bind(int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(m_Type, m_Id);
}

void Texture::Unbind() const
{
    glBindTextureUnit(0, 0);
    //glActiveTexture(0);
    //glBindTexture(m_Type, 0);
}
