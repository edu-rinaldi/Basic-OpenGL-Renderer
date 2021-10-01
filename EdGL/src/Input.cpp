#include "Input.h"

InputManager::InputManager(const Window& window, Camera* camera) :
	m_Window(window), 
	m_Camera(camera),
	m_OldMousePosition(0),
	m_FirstProcessing(true)
{
}

void InputManager::ProcessInput(float dt)
{
    bool fastSpeedMultiplier = false;
    /* --- KEYBOARD ---*/

    // l-shift
    if (m_Window.GetKeyStatus(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        fastSpeedMultiplier = true;
    // l-ctrl
    if (m_Window.GetKeyStatus(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_Camera->Move(Camera::CameraMovement::DOWN, dt, fastSpeedMultiplier);

    // W-A-S-D
    if (m_Window.GetKeyStatus(GLFW_KEY_W) == GLFW_PRESS)
        m_Camera->Move(Camera::CameraMovement::FORWARD, dt, fastSpeedMultiplier);
    if (m_Window.GetKeyStatus(GLFW_KEY_S) == GLFW_PRESS)
        m_Camera->Move(Camera::CameraMovement::BACKWARD, dt, fastSpeedMultiplier);
    if (m_Window.GetKeyStatus(GLFW_KEY_A) == GLFW_PRESS)
        m_Camera->Move(Camera::CameraMovement::LEFT, dt, fastSpeedMultiplier);
    if (m_Window.GetKeyStatus(GLFW_KEY_D) == GLFW_PRESS)
        m_Camera->Move(Camera::CameraMovement::RIGHT, dt, fastSpeedMultiplier);
    
    // Space bar
    if (m_Window.GetKeyStatus(GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Camera->Move(Camera::CameraMovement::UP, dt, fastSpeedMultiplier);
    
    // ESC
    if (m_Window.GetKeyStatus(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_Window.SetWindowShouldClose(true);

    // Mouse offset
    //if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        glm::dvec2 currentMousePosition;
        m_Window.GetCursorPosition(&currentMousePosition.x, &currentMousePosition.y);
        glm::vec2 offset = m_OldMousePosition - currentMousePosition;
        if (m_FirstProcessing)
        {
            offset = glm::vec2(0.f);
            m_FirstProcessing = false;
        } 
        
        m_OldMousePosition = currentMousePosition;
        m_Camera->Rotate(glm::vec3(offset, 0), dt);
    }
}
