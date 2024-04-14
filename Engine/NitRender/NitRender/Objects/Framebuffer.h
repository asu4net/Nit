#pragma once

namespace Nit
{
    enum class EFramebufferTextureFormat
    {
        None = 0,

        // Color
        RGBA8,
        RedInteger,

        // Depth/stencil
        Depth24Stencil8,

        // Defaults
        Depth = Depth24Stencil8
    };

    struct CFramebufferTextureSpecification
    {
        CFramebufferTextureSpecification() = default;
        CFramebufferTextureSpecification(EFramebufferTextureFormat format)
            : TextureFormat(format) {}

        EFramebufferTextureFormat TextureFormat = EFramebufferTextureFormat::None;
        // TODO: filtering/wrap
    };

    struct CFramebufferAttachmentSpecification
    {
        CFramebufferAttachmentSpecification() = default;
        CFramebufferAttachmentSpecification(std::initializer_list<CFramebufferTextureSpecification> attachments)
            : Attachments(attachments) {}

        TDynamicArray<CFramebufferTextureSpecification> Attachments = {};
    };

    struct CFramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        CFramebufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class CFramebuffer
    {
    public:
        static TSharedPtr<CFramebuffer> Create(EGraphicsAPI api, const CFramebufferSpecification& specification);
        
        virtual ~CFramebuffer() = default;
        
        virtual void Invalidate() = 0;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
        
        virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
        
        virtual const CFramebufferSpecification& GetSpecification() const = 0;
    };
}