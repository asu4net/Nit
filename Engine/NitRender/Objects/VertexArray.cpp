#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Nit::Render
{
    SharedPtr<VertexArray> VertexArray::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLVertexArray>();
        case GraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}