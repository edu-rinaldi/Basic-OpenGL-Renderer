#include "Application.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ErrorHandler.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "Input.h"
#include "Mesh.h"
#include "Model.h"
#include "Window.h"

const float FOV = 45;

int run(void)
{

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    WindowBuilder builder;
    builder.AddWindowHint(GLFW_SAMPLES, 4);
    Window window = builder.Build("Scene", 1280, 720);
    
    /* Make the window's context current */
    window.MakeContextCurrent();

    window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_FRAMEBUFFER_SRGB);  // Gamma correction
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (glewInit() != GLEW_OK)
        std::cerr << "Error initializing GLEW" << std::endl;

    ErrorHandler* errorHandler = ErrorHandler::GetInstance();
    int shaderType = 0;
    std::unique_ptr<Shader> shader;
    switch (shaderType)
    {
    case 0:
        shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/simple_fs.glsl"));
        break;
    case 1:
        shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/debug_normals_fs.glsl"));
        break;
    case 2:
        shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/debug_uv_fs.glsl"));
        break;
    case 3:
        shader = std::unique_ptr<Shader>(new Shader("res/Shaders/simple_vs.glsl", "res/Shaders/base_material_fs.glsl"));
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
    Model plane(modelPath4);
    plane.Scale(glm::vec3(10.f));
    plane.Move(glm::vec3(0.f));

    Model backpack(modelPath1);
    backpack.Move(glm::vec3(0.f, 0.5f, 0.f));
    backpack.Scale(glm::vec3(0.2f));

    Model backpack2(modelPath1);
    backpack2.Move(glm::vec3(6.f, 0.5f, 0.f));
    backpack2.Scale(glm::vec3(0.2f));

    Camera* camera = new Camera(glm::vec3(0.0f, 0.2f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    InputManager inputManager(window, camera);
    glm::mat4 projection = glm::perspective(glm::radians(FOV), window.GetWidth() / window.GetHeight(), 0.01f, 100.0f);
    shader->SetMatrix4f("u_Projection", projection);

    // Light
    shader->SetVec3("u_Light.position", backpack2.GetPosition());
    glm::vec3 lightColor = glm::vec3(1.f);
    shader->SetVec3("u_Light.ambient", glm::vec3(0.f));
    shader->SetVec3("u_Light.diffuse", lightColor);
    shader->SetVec3("u_Light.specular", lightColor);
    shader->SetFloat("u_Light.constant", 1.f);
    shader->SetFloat("u_Light.linear", 0.0014f);
    shader->SetFloat("u_Light.quadratic", 0.000007f);

    float oldTime = glfwGetTime();
    glClearColor(0.1,0.1,0.1, 1.f);
    /* Loop until the user closes the window */
    float i = 0;
    int frames = 1;
    while (!window.WindowShouldClose())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentTime = glfwGetTime();
        float dt = currentTime - oldTime;
        oldTime = currentTime;

        inputManager.ProcessInput(dt);
        shader->SetVec3("u_CameraPosition", camera->GetPosition());
        
        backpack.Rotate(1, glm::vec3(0, 1, 0));
        backpack2.Move(glm::vec3(- glm::sin(i) * dt * 10, 0, 0));
        i += 0.01;
        shader->SetVec3("u_Light.position", backpack2.GetPosition() + glm::vec3(0,0,1));
        // View
        glm::mat4 view = camera->GetViewMatrix();
        shader->SetMatrix4f("u_View", view);
        
        
        backpack.Draw(*shader);
        backpack2.Draw(*shader);
        plane.Draw(*shader);
        // Swap front and back buffers
        window.SwapBuffers();

        // Poll for and process events
        glfwPollEvents();
        frames++;
    }
    delete camera;
    glfwTerminate();
    return 0;
}