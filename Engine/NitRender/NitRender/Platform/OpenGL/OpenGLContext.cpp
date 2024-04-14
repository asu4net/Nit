#include "OpenGLContext.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Nit
{
#ifdef NIT_DEBUG
    static void APIENTRY OnDebugMessageCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
        GLsizei length, const char* message, const void* userParam)
    {
        if (!COpenGlContext::EnableDebugMessages) return;
        COpenGlContext::OnDebugMessage(message);
    }
#endif

    COpenGlContext::COpenGlContext(void* windowHandler)
        : m_WindowHandler(static_cast<GLFWwindow*>(windowHandler))
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            assert(false && "Failed to create OpenGL rendering context!");
            return;
        }

#ifdef NIT_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OnDebugMessageCallback, nullptr);
#endif
    }

    void COpenGlContext::SwapBuffers() const
    {
        glfwSwapBuffers(m_WindowHandler);
    }
}