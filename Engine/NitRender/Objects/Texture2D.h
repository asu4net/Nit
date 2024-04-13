#pragma once

namespace Nit
{
    enum class MinificationFilter  { Linear, Nearest };
    enum class MagnificationFilter { Linear, Nearest };
    enum class TextureWrapMode     { Repeat, ClampToEdge };
    enum class TextureCoordinate   { U, V };
    
    struct Texture2DSettings
    {
        uint32_t            Width     = 0;
        uint32_t            Height    = 0;
        uint32_t            Channels  = 0;
        
        MagnificationFilter MagFilter = MagnificationFilter::Linear;
        MinificationFilter  MinFilter = MinificationFilter ::Linear;
        TextureWrapMode     WrapModeU = TextureWrapMode    ::Repeat;
        TextureWrapMode     WrapModeV = TextureWrapMode    ::Repeat;
    };
}

namespace Nit::Render
{
    class Texture2D    
    {
    public:
        static SharedPtr<Texture2D> Create(GraphicsAPI api);
        virtual bool UploadToGPU(const Texture2DSettings& settings, const void* data) = 0;
        virtual uint32_t GetTextureID() const = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    using Texture2DPtr     = SharedPtr<Texture2D>;
    using WeakTexture2DPtr = WeakPtr<Texture2D>;
}