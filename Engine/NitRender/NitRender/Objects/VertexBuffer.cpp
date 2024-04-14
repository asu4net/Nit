#include "VertexBuffer.h"
#include "NitRender/Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Nit
{
    TSharedPtr<CVertexBuffer> CVertexBuffer::Create(EGraphicsAPI api, const void* vertices, const uint32_t size)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlVertexBuffer>(vertices, size);
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }

    TSharedPtr<CVertexBuffer> CVertexBuffer::Create(EGraphicsAPI api, const uint32_t size)
    {
        return CreateSharedPtr<COpenGlVertexBuffer>(size);
    }
}