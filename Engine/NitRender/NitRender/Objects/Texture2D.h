#pragma once

namespace Nit
{
    enum class EMinificationFilter  { Linear, Nearest };
    enum class EMagnificationFilter { Linear, Nearest };
    enum class ETextureWrapMode     { Repeat, ClampToEdge };
    enum class ETextureCoordinate   { U, V };
    
    struct CTexture2DSettings
    {
        uint32_t            Width     = 0;
        uint32_t            Height    = 0;
        uint32_t            Channels  = 0;
        
        EMagnificationFilter MagFilter = EMagnificationFilter::Linear;
        EMinificationFilter  MinFilter = EMinificationFilter ::Linear;
        ETextureWrapMode     WrapModeU = ETextureWrapMode    ::Repeat;
        ETextureWrapMode     WrapModeV = ETextureWrapMode    ::Repeat;
    };
}

namespace Nit
{
    class Texture2D    
    {
    public:
        static TSharedPtr<Texture2D> Create(EGraphicsAPI api);
        virtual bool UploadToGPU(const CTexture2DSettings& settings, const void* data) = 0;
        virtual uint32_t GetTextureID() const = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    using TTexture2DPtr     = TSharedPtr<Texture2D>;
    using TWeakTexture2DPtr = TWeakPtr<Texture2D>;
}