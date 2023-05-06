#pragma once
#include "Core/Asset/Asset.h"

namespace Nit
{
    enum class MinFilter { Linear, Nearest };
    enum class MagFilter { Linear, Nearest };
    enum class WrapMode { Repeat, ClampToEdge };
    enum class TextureCoordinate { U, V };
    enum class InternalFormat { None, RGB8, RGBA8 };
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

        RTTR_ENABLE()
    };
    
    class Texture2D : public Asset
    {
    public:
        Texture2D(const std::string& name, const std::string& path, const Id& id);

        bool Load() override;
        void Initialize() override;
        void UploadToGPU();
        bool Unload() override;
        
        void Configure(const Texture2DSettings& settings) { m_Settings = settings; }
        void SetData(const void* data, uint32_t size);
        
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        uint32_t GetTextureID() const { return m_TextureID; }
        
        void Bind(uint32_t slot = 0) const;

    private:
        Texture2DSettings m_Settings;
        uint32_t m_TextureID{0};
        uint32_t m_Width{0};
        uint32_t m_Height{0};
        uint32_t m_Channels{0};
        InternalFormat m_InternalFormat{InternalFormat::None};
        DataFormat m_DataFormat{DataFormat::None};
        unsigned char* m_Data{nullptr};
        bool m_bUploaded{false};
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };
}
