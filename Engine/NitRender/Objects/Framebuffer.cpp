#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Nit::Render
{
    SharedPtr<Framebuffer> Framebuffer::Create(GraphicsAPI api, const FramebufferSpecification& specification)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLFramebuffer>(specification);
        case GraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}
