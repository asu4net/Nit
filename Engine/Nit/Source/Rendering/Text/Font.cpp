#include "Font.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "Rendering/Data/Texture2D.h"

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;
    registration::class_<Font>("Font")
        .constructor<const std::string&, const std::string&, const Id&>();
}

namespace Nit
{
    Font::Font(const std::string& name, const std::string& path, const Id& id)
        : Asset(name, path, id)
    {
    }

    bool Font::Load()
    {
        std::ifstream fileStream(GetAbsolutePath(), std::ifstream::binary);

        if (!fileStream) return false;
        fileStream.seekg(0, std::ifstream::end);
        const std::streamoff length = fileStream.tellg();
        fileStream.seekg(0, std::ifstream::beg);
        char* buffer = new char[length];
        fileStream.read(buffer, length);
        fileStream.close();
        m_FontBuffer = reinterpret_cast<unsigned char*>(buffer);
        return true;
    }

    void Font::Create()
    {
        // Bake font buffer
        const int pixelsAlphaLenght = m_Height * m_Width;
        m_PixelsAlpha = new unsigned char[pixelsAlphaLenght];
        auto* bakedChar = new stbtt_bakedchar[m_CharCount];

        stbtt_BakeFontBitmap(m_FontBuffer, 0, static_cast<float>(m_PixelHeight), m_PixelsAlpha,
                             m_Width, m_Height, 0, m_CharCount, bakedChar);
        m_BakedChar = reinterpret_cast<void*>(bakedChar);
        
        const int pixelsRgbLenght = pixelsAlphaLenght * 4;
        m_PixelsRgb = new unsigned char[pixelsRgbLenght];

        int currIndex = 0;
        int pixelsIndex = 0;
        for (int i = 0; i < pixelsRgbLenght; i++)
        {
            currIndex++;
            if (currIndex == 4)
            {
                m_PixelsRgb[i] = m_PixelsAlpha[pixelsIndex];
                pixelsIndex++;
                currIndex = 0;
                continue;
            }

            m_PixelsRgb[i] = 255;
        }

        m_FontAtlas = CreateShared<Texture2D>(GetName() + "TextureAtlas", "", Id());
        Texture2DSettings settings;
        settings.CreateFromFile = false;
        settings.Width = m_Width;
        settings.Height = m_Height;
        m_FontAtlas->Configure(settings);
        m_FontAtlas->UploadToGPU();
        m_FontAtlas->SetData(m_PixelsRgb, pixelsRgbLenght * sizeof(unsigned char));
    }

    void Font::GetBakedChar(const char c, std::array<glm::vec2, 4>& vertexUV, std::array<glm::vec3, 4>& vertexPositions)
    {
        const auto* bakedChar = static_cast<stbtt_bakedchar*>(m_BakedChar);
        float xPos{0}, yPos{0};
        auto* q = new stbtt_aligned_quad();
        stbtt_GetBakedQuad(bakedChar, m_Width, m_Height, c, &xPos, &yPos, q, true);

        glm::vec2 s = { (q->s1 - q->s0), (q->t1 - q->t0) };
        s = glm::normalize(s);
        s.x /= 2;
        s.y /= 2;
        vertexPositions[0] = {-s.x, -s.y, 0}; vertexUV[0] = {q->s0, q->t1};
        vertexPositions[1] = { s.x, -s.y, 0}; vertexUV[1] = {q->s1, q->t1};
        vertexPositions[2] = { s.x,  s.y, 0}; vertexUV[2] = {q->s1, q->t0};
        vertexPositions[3] = {-s.x,  s.y, 0}; vertexUV[3] = {q->s0, q->t0};
        
        delete q;
    }

    void Font::Initialize()
    {
        Create();
    }

    bool Font::Unload()
    {
        delete[] m_PixelsAlpha;
        delete[] static_cast<stbtt_bakedchar*>(m_BakedChar);
        delete[] m_PixelsRgb;

        m_PixelsAlpha = nullptr;
        m_BakedChar = nullptr;
        m_PixelsRgb = nullptr;
        
        return true;
    }
}