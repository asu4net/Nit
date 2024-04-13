#include "API.h"
#include "Objects/VertexArray.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Nit::Render
{
    SharedPtr<API> API::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLRenderAPI>();
        case GraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}