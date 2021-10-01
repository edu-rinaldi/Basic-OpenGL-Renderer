#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    m_Id = glCreateProgram();

    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    Shader::ParseShaderFile(vertexShader, vertexShaderCode);
    Shader::ParseShaderFile(fragmentShader, fragmentShaderCode);

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    glAttachShader(m_Id, vs);
    glAttachShader(m_Id, fs);
    glLinkProgram(m_Id);
    glValidateProgram(m_Id);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glUseProgram(0);
}

Shader::~Shader()
{
    glDeleteProgram(m_Id);
}

void Shader::Bind() const { glUseProgram(m_Id); }
void Shader::Unbind() const { glUseProgram(0); }

int Shader::GetUniformLocation(const char* name) const
{
    return glGetUniformLocation(m_Id, name);
}

void Shader::SetBool(const char* name, bool value) const
{
    glProgramUniform1i(m_Id, GetUniformLocation(name), (int)value);
}
void Shader::SetInt(const char* name, int value) const
{
    glProgramUniform1i(m_Id, GetUniformLocation(name), value);
}
void Shader::SetFloat(const char* name, float value) const
{
    glProgramUniform1f(m_Id, GetUniformLocation(name), value);
}

void Shader::SetVec4(const char* name, const glm::vec4& value) const
{
    glProgramUniform4f(m_Id, GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetVec3(const char* name, const glm::vec3& value) const
{
    glProgramUniform3f(m_Id, GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetVec2(const char* name, const glm::vec2& value) const
{
    glProgramUniform2f(m_Id, GetUniformLocation(name), value.x, value.y);
}

void Shader::SetMatrix4f(const char* name, const glm::mat4& mat) const
{
    glProgramUniformMatrix4fv(m_Id, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) const
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int shaderCompiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE)
    {
        char infoLog[1024];
        int infoLogLength;
        glGetShaderInfoLog(id, 1024, &infoLogLength, infoLog);
        std::cerr << GetType(type) << " shader error: " << std::endl;
        std::cerr << infoLog << std::endl;
    }

    return id;
}

void Shader::ParseShaderFile(const std::string& fname, std::string& outShader)
{
    std::ifstream shaderFStream(fname, std::ios::in);
    if (shaderFStream.is_open())
    {
        std::stringstream strStream;
        strStream << shaderFStream.rdbuf();
        outShader = strStream.str();
        shaderFStream.close();
    }
    else
    {
        throw std::invalid_argument("Non existing input shader file at: " + fname) ;
    }
}

std::string Shader::GetType(unsigned int type)
{
    switch (type)
    {
        case GL_FRAGMENT_SHADER: return "Fragment";
        case GL_VERTEX_SHADER: return "Vertex";
        case GL_GEOMETRY_SHADER: return "Geometry";
        default: return "Invalid";
    }
}

