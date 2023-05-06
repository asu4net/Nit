#pragma once
#include "Core/Asset/Asset.h"

namespace Nit
{
    class Font : public Asset
    {
    public:
        Font(const std::string& name, const std::string& path, const Id& id);
        
        bool Load() override;
        void Initialize() override;
        bool Unload() override;
        void Create();

        glm::vec2 GetCharLocation(char c);

        const Shared<class Texture2D>& GetFontAtlas() const { return m_FontAtlas; } 
        
    private:
        uint32_t m_Height = 1024;
        uint32_t m_Width = 1024;
        uint32_t m_PixelHeight = 32;
        uint32_t m_CharCount = 256;
        unsigned char* m_FontBuffer = nullptr;
        void* m_BakedChar = nullptr;
        unsigned char* m_PixelsRgb = nullptr;
        unsigned char* m_PixelsAlpha = nullptr;
        Shared<class Texture2D> m_FontAtlas;

        RTTR_ENABLE(Asset)
    };
}
