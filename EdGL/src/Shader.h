#pragma once

// Includes
#include <iostream>
#include <glm/glm.hpp>

namespace edgl {

class Shader
{
	unsigned int m_Id;

public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	void Bind() const;
	void Unbind() const;

	int GetUniformLocation(const char* name) const;

	void SetBool(const char* name, bool value) const;
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;

	void SetVec4(const char* name, const glm::vec4& value) const;
	void SetVec3(const char* name, const glm::vec3& value) const;
	void SetVec2(const char* name, const glm::vec2& value) const;

	void SetMatrix4f(const char* name, const glm::mat4& mat) const;

	unsigned int CompileShader(unsigned int type, const std::string& source) const;

	static void ParseShaderFile(const std::string& fname, std::string& outShader);

	static std::string GetType(unsigned int type);
};

}