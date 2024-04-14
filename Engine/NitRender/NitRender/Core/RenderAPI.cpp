#include "NitRender/Core/RenderAPI.h"
#include "NitRender/Objects/VertexArray.h"
#include "NitRender/Platform/OpenGL/OpenGLRenderAPI.h"

namespace Nit
{
    TSharedPtr<CRenderAPI> CRenderAPI::Create(EGraphicsAPI api)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlRenderAPI>();
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}