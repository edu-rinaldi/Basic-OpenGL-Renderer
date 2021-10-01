#pragma once
#include <gl/glew.h>

#ifdef DEBUG_MODE
    #define ASSERT(x) if(!x) __debugbreak()
#else
    #define  ASSERT(x)
#endif

class ErrorHandler
{
private:
    bool m_Enabled;
    static ErrorHandler* m_Instance;
    ErrorHandler();
    static void GLAPIENTRY OpenglCallbackFunction(GLenum source, GLenum type, GLuint id, 
        GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
public:
    ErrorHandler(const ErrorHandler& obj) = delete;
    static ErrorHandler* GetInstance();
    
    void Enable();
    void Disable();

    inline bool IsEnabled() { return m_Enabled; }
    
};



