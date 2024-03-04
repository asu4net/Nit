#include "RendererAPI.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Nit
{
    SharedPtr<RendererAPI> RendererAPI::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLRenderAPI>();
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}