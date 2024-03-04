#include "RendererTexture2D.h"
#include "Platform\OpenGL\OpenGLTexture2D.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    rttr::registration::class_<Texture2DSettings>("Texture2DSettings")
        .constructor<>()
        .property("MagFilter", &Texture2DSettings::MagFilter)
        .property("MinFilter", &Texture2DSettings::MinFilter)
        .property("WrapModeU", &Texture2DSettings::WrapModeU)
        .property("WrapModeV", &Texture2DSettings::WrapModeV);
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(Texture2DSettings)

    SharedPtr<RendererTexture2D> RendererTexture2D::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLTexture2D>();
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}