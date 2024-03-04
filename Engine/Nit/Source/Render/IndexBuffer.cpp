#include "IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Nit
{
    SharedPtr<IndexBuffer> IndexBuffer::Create(GraphicsAPI api, const uint32_t* indices, const uint32_t count)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLIndexBuffer>(indices, count);
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}