#pragma once
#include "Render/Framebuffer.h"

namespace Nit
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& specification);
        ~OpenGLFramebuffer() override;

        void Invalidate() override;
        void Bind() const override;
        void Unbind() const override;
        void Resize(uint32_t width, uint32_t height) override;

        int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
        void ClearAttachment(uint32_t attachmentIndex, int value) override;
        uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { 
            NIT_CHECK((index < (uint32_t) m_ColorAttachments.size()), "Invalid color attachment!"); return m_ColorAttachments[index];
        };
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

    private:
        uint32_t m_FrameBufferId = 0;
        FramebufferSpecification m_Specification;

        DynamicArray<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

        DynamicArray<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}