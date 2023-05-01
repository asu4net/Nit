#pragma once
#include "Texture.h"
#include "Core/Asset/Asset.h"

namespace Nit
{
    enum class MinFilter { Linear, Nearest };
    enum class MagFilter { Linear, Nearest };
    enum class WrapMode { Repeat, ClampToEdge };
    enum class TextureCoordinate { U, V };
    enum class InternalFormat { None, RGB8 };
    enum class DataFormat { None, RGB, RGBA };
    
    struct Texture2DSettings
    {
        bool CreateFromFile{true};
        uint32_t Width{0};
        uint32_t Height{0};
        MagFilter MagFilter{MagFilter::Linear};
        MinFilter MinFilter{MinFilter::Linear};
        WrapMode WrapModeU{WrapMode::Repeat};
        WrapMode WrapModeV{WrapMode::Repeat};
    };
    
    class Texture2D : public Texture, public Asset
    {
    public:
        Texture2D(const std::string& name, const std::string& path, const Id& id);

        bool Load() override;
        void UploadToGPU(const Texture2DSettings& settings ={});
        bool Unload() override;
        
        void SetData(const void* data, uint32_t size);
        
        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        uint32_t GetTextureID() const override { return m_TextureID; }
        
        void Bind(uint32_t slot = 0) const override;

    private:
        uint32_t m_TextureID{0};
        uint32_t m_Width{0};
        uint32_t m_Height{0};
        uint32_t m_Channels{0};
        InternalFormat m_InternalFormat{InternalFormat::None};
        DataFormat m_DataFormat{DataFormat::None};
        unsigned char* m_Data{nullptr};
        RTTR_ENABLE(Asset)
    };
}
