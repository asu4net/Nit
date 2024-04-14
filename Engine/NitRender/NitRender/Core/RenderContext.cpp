#include "RenderContext.h"
#include "NitRender/Platform/OpenGL/OpenGLContext.h"

namespace Nit
{
    TUniquePtr<CRenderContext> CRenderContext::Create(EGraphicsAPI api, void* windowHandler)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateUniquePtr<COpenGlContext>(windowHandler);
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}