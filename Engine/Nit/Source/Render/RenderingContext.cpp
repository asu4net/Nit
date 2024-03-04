#include "RenderingContext.h"
#include "Platform/OpenGL/OpenGLRenderingContext.h"

namespace Nit
{
    UniquePtr<RenderingContext> RenderingContext::Create(GraphicsAPI api, void* windowHandler)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateUniquePtr<OpenGLRenderingContext>(windowHandler);
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}