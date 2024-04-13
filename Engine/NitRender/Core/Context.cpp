#include "Context.h"
#include "Platform\OpenGL\OpenGLContext.h"

namespace Nit::Render
{
    UniquePtr<Context> Context::Create(GraphicsAPI api, void* windowHandler)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateUniquePtr<OpenGLContext>(windowHandler);
        case GraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}