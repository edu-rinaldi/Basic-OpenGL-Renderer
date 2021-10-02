#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"

namespace edgl {

Application::Application(ApplicationSettings settings) : m_OldTime(0), m_CurrentTime(0), m_DT(0), m_Settings(settings)
{
    if(!Init(settings)) throw ApplicationCreationException();
}

void Application::OnInit() {}
void Application::OnLoop(float dt) {}

bool Application::Init(const ApplicationSettings& settings)
{
    if(!glfwInit()) 
    {
        // Log error
        return false;
    }

    WindowBuilder windowBuilder;
    if(settings.m_MultiSample) windowBuilder.AddWindowHint(GLFW_SAMPLES, settings.m_Samples);
    
    windowBuilder.AddWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    windowBuilder.AddWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_Window = windowBuilder.Build(settings.m_Name.c_str(), static_cast<float>(settings.m_WindowWidth), static_cast<float>(settings.m_WindowHeight));
    m_Window->MakeContextCurrent();
    m_Window->SetInputMode(GLFW_CURSOR, settings.m_CursorsMode);

    // Depth test
    if(settings.m_DepthTest) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    // Gamma correction
    if(settings.m_GammaCorrection) glEnable(GL_FRAMEBUFFER_SRGB);
    else glDisable(GL_FRAMEBUFFER_SRGB);

    // Blend function
    if(settings.m_Blend) 
    { 
        glEnable(GL_BLEND);
        glBlendFunc(settings.m_BlendFunction.sourceFactor, settings.m_BlendFunction.destinationFactor);
    }
    else glDisable(GL_BLEND);

    if(glewInit() != GLEW_OK) 
    {
        // Log error
        return false;
    }

    glClearColor(settings.m_ClearColor.r,settings.m_ClearColor.g, settings.m_ClearColor.b, settings.m_ClearColor.a);
    printf("GL Version: %s \n", glGetString(GL_VERSION));

    return true;
}

void Application::Loop()
{
    while(!m_Window->WindowShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Delta
        m_CurrentTime = glfwGetTime();
        m_DT = m_CurrentTime - m_OldTime;
        m_OldTime = m_CurrentTime;

        OnLoop(m_DT);

        // Swap front and back buffers
        m_Window->SwapBuffers();
        // Poll for and process events
        glfwPollEvents();

    }
    glfwTerminate();

}

void Application::Run() { OnInit(); Loop(); }

}
