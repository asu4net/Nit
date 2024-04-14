#include "VertexArray.h"
#include "NitRender/Platform/OpenGL/OpenGLVertexArray.h"

namespace Nit
{
    TSharedPtr<CVertexArray> CVertexArray::Create(EGraphicsAPI api)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlVertexArray>();
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}