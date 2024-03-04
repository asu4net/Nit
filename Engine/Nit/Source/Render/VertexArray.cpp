#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Nit
{
    SharedPtr<VertexArray> VertexArray::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLVertexArray>();
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}