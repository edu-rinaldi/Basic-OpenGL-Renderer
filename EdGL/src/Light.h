#pragma once
#include <string>
#include <glm/vec3.hpp>

namespace edgl {

class Shader;

class Light
{
public:
    virtual void AddToShader(Shader& shader) const;
protected:
    Light(const std::string& name,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
        float constant, float linear, float quadratic);

    std::string m_Name;
    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;

    bool m_HasPosition;
    bool m_HasDirection;

    float m_Constant;
    float m_Linear;
    float m_Quadratic;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(const std::string& name, const glm::vec3& direction,
                    const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                    float constant, float linear, float quadratic);

    virtual void AddToShader(Shader& shader) const override;
    inline const glm::vec3& GetDirection() { return m_Direction; }
private:
    glm::vec3 m_Direction;

};

class PointLight : public Light
{
public:
    PointLight(const std::string& name, const glm::vec3& position,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
        float constant, float linear, float quadratic);

    virtual void AddToShader(Shader& shader) const override;
    inline const glm::vec3& GetPosition() { return m_Position; }
private:
    glm::vec3 m_Position;
};

class SpotLight : public Light
{
public:
    SpotLight(const std::string& name, const glm::vec3& position, const glm::vec3& direction, float cutOff,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
        float constant, float linear, float quadratic);

    virtual void AddToShader(Shader& shader) const override;
    
    inline const glm::vec3& GetPosition() { return m_Position; }
    inline const glm::vec3& GetDirection() { return m_Direction; }
private:
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    float m_CutOff;
};

}
