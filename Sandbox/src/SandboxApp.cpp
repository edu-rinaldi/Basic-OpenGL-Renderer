#include "SandboxApp.h"

#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"

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

    settings.m_CullFace = true;
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
    auto modelPath1 = "res/Models/backpack/backpack.obj";
    auto modelPath2 = "res/Models/watchtower/wooden watch tower2.obj";
    auto modelPath3 = "res/Models/tv/retrotv0319.obj";
    auto modelPath4 = "res/Models/floor/floor.obj";
    auto modelPath5 = "res/Models/mercedes/mercedes.obj";
    auto bb8Path = "res/Models/bb8/BB-8.obj";
    auto bunnyPath = "res/Models/bunny.ply";
    auto spherePath = "res/Models/sphere/sphere.obj";
    auto whiteTexture = Texture2D::FlatColor(glm::vec3(1.f), 1, 1, "white");
    auto specMaterial = std::shared_ptr<Material>(
        new Material(
            glm::vec3(0.2f), whiteTexture,
            glm::vec3(0.7f), Texture2D::FromFile("res/Texture/uvgrid1.png", "uvgrid"),
            glm::vec3(1.f), whiteTexture,
            32)
        );

    auto plane = std::make_shared<Model>(modelPath4);
    plane->Scale(glm::vec3(30.f));
    plane->Move(glm::vec3(0.f));
    plane->EnableCullFace(false);
    m_Models.push_back(plane);
    
    auto backpack = std::make_shared<Model>(modelPath1);
    backpack->Move(glm::vec3(0.f, 0.5f, 0.f));
    backpack->Scale(glm::vec3(0.2f));
    m_Models.push_back(backpack);
    
    backpack->EnableCullFace(false);

    auto bunny = std::make_shared<Model>(bunnyPath);
    bunny->Move(glm::vec3(2.f, 0.5f, 0.f));
    bunny->Scale(glm::vec3(5.f));
    bunny->ApplyMaterial(specMaterial);
    m_Models.push_back(bunny);
    bunny->EnableCullFace(false);
    
    // Directional light
    auto dirLightCol = glm::vec3(0.f);
    auto directionalLight = std::make_shared<DirectionalLight>("u_Light[0]", glm::vec3(0, -1, -1), glm::vec3(0.0f), dirLightCol, dirLightCol);
    m_Lights.push_back(directionalLight);
    directionalLight->AddToShader(*m_Shader);
    
    // Point light
    glm::vec3 colors[] = { glm::vec3(0,0,1), glm::vec3(0,1,0), glm::vec3(1,0,0), glm::vec3(1,1,0), glm::vec3(1,0,1) };
    for (int i = 1; i < 6; i++)
    {
        auto lcolor = colors[i - 1];
        auto lpos = glm::gaussRand(backpack->GetPosition(), glm::vec3(1, 0, 1));
        auto pointLight = std::make_shared<PointLight>("u_Light[" + std::to_string(i) + "]", 
            lpos, 
            glm::vec3(0), 
            lcolor, lcolor, 
            1, 0.7f, 1.8f);

        m_Lights.push_back(pointLight);
        pointLight->AddToShader(*m_Shader);
    }

    // Spot light
    glm::vec3 pos[] =  { glm::vec3(2,2,2), glm::vec3(3,1,2), glm::vec3(-2,2,-2), glm::vec3(1,1,-1) };
    for (int i = 6; i < 9; ++i)
    {
        auto lcolor = colors[i - 6];
        auto lpos = pos[i - 6];

        auto spotLight = std::make_shared<SpotLight>("u_Light[" + std::to_string(i) + "]", 
            lpos, glm::vec3(0,-1,0), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(19.5f)),
            glm::vec3(0),
            lcolor, lcolor);

        m_Lights.push_back(spotLight);
        spotLight->AddToShader(*m_Shader);
    }

    auto spotLight = std::make_shared<SpotLight>("u_Light[9]",
        backpack->GetPosition() + glm::vec3(0,1,0) * 4.f + glm::vec3(1,0,0) * 1.f, glm::vec3(0,-1,0), 
        glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(18.5f)),
        glm::vec3(0),
        glm::vec3(1, 0.94, 0.8), glm::vec3(1, 0.94, 0.8));

    m_Lights.push_back(spotLight);
    spotLight->AddToShader(*m_Shader);

    m_Shader->SetMatrix4f("u_Projection", m_Projection);
}

static float tmp = 0;
void SandboxApp::OnLoop(float dt)
{
    m_InputManager->ProcessInput(dt);
    m_Shader->SetVec3("u_CameraPosition", m_Camera->GetPosition());
    
    auto backpack = m_Models[1];
    auto bunny = m_Models[2];

    backpack->Rotate(1, glm::vec3(0, 1, 0));
    bunny->Rotate(-1, glm::vec3(0, 1, 0));
    //bunny->Move(glm::vec3(- glm::sin(tmp) * dt * 2, 0, 0));
    tmp += 0.01;
    m_Shader->SetVec3("u_Light[1].position", bunny->GetPosition() + glm::vec3(0,0,1) * 0.5f);

    // View
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader->SetMatrix4f("u_View", view);
    
    for(auto model : m_Models)
        model->Draw(*m_Shader);

}