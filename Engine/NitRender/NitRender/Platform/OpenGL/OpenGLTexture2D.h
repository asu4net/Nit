#pragma once
#include "NitRender/Objects/Texture2D.h"

namespace Nit
{
    class COpenGlTexture2D : public Texture2D
    {
    public:
        COpenGlTexture2D();
        ~COpenGlTexture2D();

        bool UploadToGPU(const CTexture2DSettings& settings, const void* data) override;
        uint32_t GetTextureID() const override { return m_TextureID; }
        void Bind(uint32_t slot = 0) const override;

    private:
        uint32_t m_TextureID = 0;
        bool m_Uploaded = false;
    };
}