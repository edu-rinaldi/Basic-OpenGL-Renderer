#pragma once
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace edgl {

class Shader;
class Model;

class DrawableLight
{
    virtual void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const = 0;
};

class Light
{
public:
    virtual void AddToShader(Shader& shader) const;
protected:
    Light(const std::string& name,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    std::string m_Name;
    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;

    bool m_HasPosition;
    bool m_HasDirection;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(const std::string& name, const glm::vec3& direction,
                    const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

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
    float m_Constant;
    float m_Linear;
    float m_Quadratic;
};

class SpotLight : public Light
{
public:
    SpotLight(const std::string& name, const glm::vec3& position, const glm::vec3& direction, 
        float cutOff,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    SpotLight(const std::string& name, const glm::vec3& position, const glm::vec3& direction, 
        float cutOff, float outCutOff,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    virtual void AddToShader(Shader& shader) const override;
    
    inline const glm::vec3& GetPosition() { return m_Position; }
    inline const glm::vec3& GetDirection() { return m_Direction; }
private:
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    float m_CutOff;
    float m_OutCutOff;
};

}
