#pragma once

#include "NitRender/Core/RenderContext.h"

struct GLFWwindow;

namespace Nit
{
    class COpenGlContext : public CRenderContext
    {
    public:
#ifdef NIT_DEBUG
        inline static bool EnableDebugMessages = false;
        inline static TFunctionPtr<void(const TString& message)> OnDebugMessage = [](const TString& message)
        {
           //NIT_LOG_TRACE("[OpenGL] %s \n", message.c_str());
        };
#endif
        COpenGlContext(void* windowHandler);

        void SwapBuffers() const override;

    private:
        GLFWwindow* m_WindowHandler;
    };
}