#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Nit
{
    SharedPtr<Framebuffer> Framebuffer::Create(GraphicsAPI api, const FramebufferSpecification& specification)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLFramebuffer>(specification);
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}
