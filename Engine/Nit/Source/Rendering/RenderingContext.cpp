#include "RenderingContext.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Nit
{
#ifdef SH_DEBUG
    static void APIENTRY OnDebugMessageCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
            GLsizei length, const char* message, const void* userParam)
    {
        if (!RenderingContext::EnableDebugMessages) return;
        RenderingContext::OnDebugMessage(message);
    }
#endif

    static GLFWwindow* GetGlfWwindow(void* windowHandler)
    {
        static GLFWwindow* glfWwindowHandler = static_cast<GLFWwindow*>(windowHandler);
        return glfWwindowHandler;
    }
    
    Shared<RenderingContext> InstantiateContext(void* windowHandler)
    {
        return std::make_shared<RenderingContext>(windowHandler);
    }

    RenderingContext::RenderingContext(void* windowHandler)
        : m_WindowHandler(windowHandler)
    {
    }

    void RenderingContext::Initialize()
    {
        assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) && "Failed to initialize glad!");
        
#ifdef SH_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OnDebugMessageCallback, nullptr);
#endif
    }

    void RenderingContext::SwapBuffers() const
    {
        glfwSwapBuffers(GetGlfWwindow(m_WindowHandler));
    }
}
