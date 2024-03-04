#pragma once
#include "Render/RendererTexture2D.h"

namespace Nit
{
    class OpenGLTexture2D : public RendererTexture2D
    {
    public:
        OpenGLTexture2D();
        ~OpenGLTexture2D();

        void UploadToGPU(const Texture2DSettings& settings, const void* data) override;
        uint32_t GetTextureID() const override { return m_TextureID; }
        void Bind(uint32_t slot = 0) const override;

    private:
        uint32_t m_TextureID = 0;
        bool m_Uploaded = false;
    };
}