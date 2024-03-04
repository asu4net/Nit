#pragma once
#include "Render/RenderingContext.h"

struct GLFWwindow;

namespace Nit
{
    class OpenGLRenderingContext : public RenderingContext
    {
    public:
#ifdef NIT_DEBUG
        inline static bool EnableDebugMessages = false;
        inline static std::function<void(const String& message)> OnDebugMessage = [](const String& message)
        {
           NIT_LOG_TRACE("[OpenGL] %s \n", message.c_str());
        };
#endif
        OpenGLRenderingContext(void* windowHandler);

        void SwapBuffers() const override;

    private:
        GLFWwindow* m_WindowHandler;
    };
}