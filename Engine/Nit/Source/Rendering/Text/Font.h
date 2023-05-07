#pragma once
#include "Core/Asset/Asset.h"

namespace Nit
{

    struct AlignedQuad
    {
        float X0,Y0,S0,T0; // top-left
        float X1,Y1,S1,T1; // bottom-right
        float XPos, YPos;
    } ;
    
    class Font : public Asset
    {
    public:
        Font(const std::string& name, const std::string& path, const Id& id);
        
        bool Load() override;
        void Initialize() override;
        bool Unload() override;
        void Create();

        void GetBakedChar(char c, AlignedQuad& q);

        const Shared<class Texture2D>& GetFontAtlas() const { return m_FontAtlas; } 
        
    private:
        uint32_t m_Height = 1024;
        uint32_t m_Width = 1024;
        uint32_t m_PixelHeight = 64;
        uint32_t m_CharCount = 256;
        unsigned char* m_FontBuffer = nullptr;
        void* m_BakedChar = nullptr;
        unsigned char* m_PixelsRgb = nullptr;
        unsigned char* m_PixelsAlpha = nullptr;
        Shared<class Texture2D> m_FontAtlas;

        RTTR_ENABLE(Asset)
    };
}
