#pragma once
#include "Camera.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace edgl {

class InputManager
{
private:
    Window m_Window;
    Camera* m_Camera;

    glm::dvec2 m_OldMousePosition;
    bool m_FirstProcessing;
public:
    InputManager(const Window& window, Camera* camera);

    void ProcessInput(float dt);


};

}