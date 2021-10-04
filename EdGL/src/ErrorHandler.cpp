#include "ErrorHandler.h"
#include <iostream>

namespace edgl {

    ErrorHandler* ErrorHandler::m_Instance = nullptr;

    ErrorHandler::ErrorHandler() : m_Enabled(true)
    {
        glDebugMessageCallback(OpenglCallbackFunction, 0);
        glEnable(GL_DEBUG_OUTPUT);
    }

    ErrorHandler* ErrorHandler::GetInstance()
    {
        if (m_Instance == nullptr) m_Instance = new ErrorHandler();
        return m_Instance;
    }


    void ErrorHandler::Enable()
    {
        glEnable(GL_DEBUG_OUTPUT);
        m_Enabled = true;
    }

    void ErrorHandler::Disable()
    {
        glDisable(GL_DEBUG_OUTPUT);
        m_Enabled = false;
    }


    void ErrorHandler::OpenglCallbackFunction(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        std::cerr << "---------------------opengl-callback-start------------" << std::endl;
        std::cerr << "message: " << message << std::endl;
        std::cerr << "type: ";
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cerr << "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cerr << "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cerr << "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cerr << "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cerr << "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cerr << "OTHER";
            break;
        }
        std::cerr << std::endl;

        std::cerr << "id: " << id << std::endl;
        std::cerr << "severity: ";
        switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
            std::cerr << "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "HIGH";
            break;
        default: std::cerr << severity;
        }
        std::cerr << std::endl;
        std::cerr << "---------------------opengl-callback-end--------------" << std::endl;
    }

}