#pragma once
#include <glm/vec4.hpp>
#include "Util.h"
#include "Input.h"

namespace edgl {

struct ApplicationSettings
{
    std::string m_Name;
    
    int m_Samples;
    bool m_MultiSample;

    int m_WindowWidth;
    int m_WindowHeight;
    
    int m_CursorsMode;

    bool m_DepthTest;

    bool m_GammaCorrection;
    
    bool m_Blend;
    struct { unsigned int sourceFactor; unsigned int destinationFactor; } m_BlendFunction;

    bool m_CullFace;
    unsigned int m_CullFaceType;

    glm::vec4 m_ClearColor;

    float m_Fov;

};

class Application
{
public:
    Application(const Application& app) = delete;

    Application(ApplicationSettings settings);
    inline float GetDeltaTime() { return m_DT; }
    virtual void OnInit();
    virtual void OnLoop(float dt);
    void Run();
protected:
    Ref<Window> m_Window;
    ApplicationSettings m_Settings;
    inline virtual const Ref<InputManager>& GetInputManager() const = 0;
private:
    float m_OldTime;
    float m_CurrentTime;
    float m_DT;
    bool Init(const ApplicationSettings& settings);
    void Loop();
};


class ApplicationCreationException : public std::exception
{
  inline virtual const char* what() const throw() { return "ApplicationCreationException: cannot init Application"; }
};

}