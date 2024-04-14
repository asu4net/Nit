#include "Framebuffer.h"
#include "NitRender/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Nit
{
    TSharedPtr<CFramebuffer> CFramebuffer::Create(EGraphicsAPI api, const CFramebufferSpecification& specification)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlFramebuffer>(specification);
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}
