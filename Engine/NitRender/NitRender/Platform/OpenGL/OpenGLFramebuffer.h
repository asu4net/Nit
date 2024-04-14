#pragma once
#include "NitRender/Objects/Framebuffer.h"

namespace Nit
{
    class COpenGlFramebuffer : public CFramebuffer
    {
    public:
        COpenGlFramebuffer(const CFramebufferSpecification& specification);
        ~COpenGlFramebuffer() override;

        void Invalidate() override;
        void Bind() const override;
        void Unbind() const override;
        void Resize(uint32_t width, uint32_t height) override;

        int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
        void ClearAttachment(uint32_t attachmentIndex, int value) override;
        uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
            assert((index < (uint32_t) m_ColorAttachments.size()) && "Invalid color attachment!");
            return m_ColorAttachments[index];
        };
        const CFramebufferSpecification& GetSpecification() const override { return m_Specification; }

    private:
        uint32_t m_FrameBufferId = 0;
        CFramebufferSpecification m_Specification;

        TDynamicArray<CFramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        CFramebufferTextureSpecification m_DepthAttachmentSpecification = EFramebufferTextureFormat::None;

        TDynamicArray<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}