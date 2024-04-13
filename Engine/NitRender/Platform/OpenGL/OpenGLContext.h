#pragma once

#include "Core/Context.h"

struct GLFWwindow;

namespace Nit::Render
{
    class OpenGLContext : public Context
    {
    public:
#ifdef NIT_DEBUG
        inline static bool EnableDebugMessages = false;
        inline static FunctionPtr<void(const String& message)> OnDebugMessage = [](const String& message)
        {
           //NIT_LOG_TRACE("[OpenGL] %s \n", message.c_str());
        };
#endif
        OpenGLContext(void* windowHandler);

        void SwapBuffers() const override;

    private:
        GLFWwindow* m_WindowHandler;
    };
}