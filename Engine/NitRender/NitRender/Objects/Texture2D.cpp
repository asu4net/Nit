#include "Texture2D.h"
#include "NitRender/Platform/OpenGL/OpenGLTexture2D.h"

namespace Nit
{
    TSharedPtr<Texture2D> Texture2D::Create(EGraphicsAPI api)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlTexture2D>();
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}