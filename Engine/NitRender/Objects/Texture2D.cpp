#include "Texture2D.h"
#include "Platform\OpenGL\OpenGLTexture2D.h"

namespace Nit::Render
{
    SharedPtr<Texture2D> Texture2D::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLTexture2D>();
        case GraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}