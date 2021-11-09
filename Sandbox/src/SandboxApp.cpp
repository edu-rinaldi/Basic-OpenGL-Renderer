#include "SandboxApp.h"

#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"
#include "Util.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

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
    settings.m_Name = "Sandbox test application @ EdGL 0.0.1";
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

    settings.m_CullFace = false;
    settings.m_CullFaceType = GL_FRONT;

    settings.m_Fov = 60.f;
    return settings;
}


void SandboxApp::OnInit()
{
    int shaderType = 4;
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
    case 4:
        m_Shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/multi_light_fs.glsl"));
        break;
    default:
        break;
    }

    // Models
    auto backpackModelPath = "res/Models/backpack/backpack.obj";
    auto floorModelPath = "res/Models/floor/floor.obj";
    auto bunnyPath = "res/Models/bunny.ply";
    auto whiteTexture = Texture2D::FlatColor(glm::vec3(1.f), 1, 1, "white");
    auto specMaterial = edgl::Ref<Material>(
        new Material(
            glm::vec3(0.2f), whiteTexture,
            glm::vec3(0.7f), Texture2D::FromFile("res/Texture/uvgrid1.png", "uvgrid"),
            glm::vec3(1.f), whiteTexture,
            32)
        );
    // Plane model
    auto plane = std::make_shared<Model>(floorModelPath);
    plane->Scale(glm::vec3(30.f));
    plane->Move(glm::vec3(0.f));
    plane->ApplyMaterial(specMaterial);
    m_Models.push_back(plane);
    
    // Backpack model
    auto backpack = std::make_shared<Model>(backpackModelPath);
    backpack->Move(glm::vec3(0.f, 0.5f, 0.f));
    backpack->Scale(glm::vec3(0.2f));
    m_Models.push_back(backpack);

    // Bunny model
    auto bunny = std::make_shared<Model>(bunnyPath);
    bunny->Move(glm::vec3(2.f, 0.5f, 0.f));
    bunny->Scale(glm::vec3(5.f));
    bunny->ApplyMaterial(specMaterial);
    m_Models.push_back(bunny);
    
    // Directional light
    auto dirLightCol = glm::vec3(1.f);
    auto directionalLight = std::make_shared<DirectionalLight>("u_DirectionalLight", glm::vec3(1, -1, -1), glm::vec3(0.0f), dirLightCol, dirLightCol);
    m_Lights.push_back(directionalLight);
    directionalLight->AddToShader(*m_Shader);
    
    m_Shader->SetMatrix4f("u_Projection", m_Projection);
}

void SandboxApp::OnLoop(float dt)
{
    m_InputManager->ProcessInput(dt);
    m_Shader->SetVec3("u_CameraPosition", m_Camera->GetPosition());

    // View
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader->SetMatrix4f("u_View", view);
    
    for(auto model : m_Models)
        model->Draw(*m_Shader);

}