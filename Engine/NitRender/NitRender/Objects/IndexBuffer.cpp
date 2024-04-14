#include "IndexBuffer.h"
#include "NitRender/Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Nit
{
    TSharedPtr<CIndexBuffer> CIndexBuffer::Create(EGraphicsAPI api, const uint32_t* indices, const uint32_t count)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlIndexBuffer>(indices, count);
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}