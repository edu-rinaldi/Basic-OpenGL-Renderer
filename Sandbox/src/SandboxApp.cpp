#include "SandboxApp.h"

#include "Shader.h"
#include "Model.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

SandboxApp::SandboxApp() : 
    Application(GetSettings()),
    m_Camera(std::make_shared<Camera>(glm::vec3(0.0f, 0.2f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
    m_Projection(glm::perspective(glm::radians(m_Settings.m_Fov), m_Window->GetWidth() / m_Window->GetHeight(), 0.01f, 100.0f))
{
    m_InputManager = std::make_shared<InputManager>(*m_Window, m_Camera.get());
}

edgl::ApplicationSettings SandboxApp::GetSettings()
{
    edgl::ApplicationSettings settings;
    settings.m_Name = "Sandbox application";
    settings.m_Samples = 4;
    settings.m_MultiSample = true;

    settings.m_WindowWidth = 1280;
    settings.m_WindowHeight = 720;

    settings.m_GammaCorrection = false;
    settings.m_DepthTest = true;
    settings.m_CursorsMode = GLFW_CURSOR_DISABLED;

    settings.m_ClearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);

    settings.m_Blend = true;
    settings.m_BlendFunction = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };

    settings.m_Fov = 45.f;
    return settings;
}


void SandboxApp::OnInit()
{
    int shaderType = 0;
    switch (shaderType)
    {
    case 0:
        m_Shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/simple_fs.glsl"));
        break;
    case 1:
        m_Shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/debug_normals_fs.glsl"));
        break;
    case 2:
        m_Shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/debug_uv_fs.glsl"));
        break;
    case 3:
        m_Shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/base_material_fs.glsl"));
        break;
    default:
        break;
    }

    // Models
    auto modelPath1 = "res/Models/backpack/backpack.obj";
    auto modelPath2 = "res/Models/watchtower/wooden watch tower2.obj";
    auto modelPath3 = "res/Models/tv/retrotv0319.obj";
    auto modelPath4 = "res/Models/floor/floor.obj";
    auto modelPath5 = "res/Models/mercedes/mercedes.obj";
    auto bb8Path = "res/Models/bb8/BB-8.obj";

    auto plane = std::make_shared<Model>(modelPath4);
    plane->Scale(glm::vec3(10.f));
    plane->Move(glm::vec3(0.f));
    m_Models.push_back(plane);

    auto backpack = std::make_shared<Model>(modelPath1);
    backpack->Move(glm::vec3(0.f, 0.5f, 0.f));
    backpack->Scale(glm::vec3(0.2f));
    m_Models.push_back(backpack);

    auto backpack2 = std::make_shared<Model>(modelPath1);
    backpack2->Move(glm::vec3(2.f, 0.5f, 0.f));
    backpack2->Scale(glm::vec3(0.2f));
    m_Models.push_back(backpack2);

    m_Shader->SetMatrix4f("u_Projection", m_Projection);
    // Light
    m_Shader->SetVec3("u_Light.position", glm::vec3(0,2.f,0));
    glm::vec3 lightColor = glm::vec3(1.f);
    m_Shader->SetVec3("u_Light.ambient", glm::vec3(0.f));
    m_Shader->SetVec3("u_Light.diffuse", lightColor);
    m_Shader->SetVec3("u_Light.specular", lightColor);
    m_Shader->SetFloat("u_Light.constant", 1.f);
    m_Shader->SetFloat("u_Light.linear", 0.0014f);
    m_Shader->SetFloat("u_Light.quadratic", 0.000007f);

}

static float tmp = 0;
void SandboxApp::OnLoop(float dt)
{
    m_InputManager->ProcessInput(dt);
    m_Shader->SetVec3("u_CameraPosition", m_Camera->GetPosition());
    
    auto backpack = m_Models[1];
    auto backpack2 = m_Models[2];

    backpack->Rotate(1, glm::vec3(0, 1, 0));
    backpack2->Move(glm::vec3(- glm::sin(tmp) * dt * 10, 0, 0));
    tmp += 0.01;
    m_Shader->SetVec3("u_Light.position", backpack2->GetPosition());
    // View
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader->SetMatrix4f("u_View", view);
    
    for(auto model : m_Models)
        model->Draw(*m_Shader);

}