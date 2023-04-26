#pragma once
#include "Texture.h"

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
        MagFilter MagFilter = MagFilter::Linear;
        MinFilter MinFilter = MinFilter::Linear;
        WrapMode WrapModeU = WrapMode::Repeat;
        WrapMode WrapModeV = WrapMode::Repeat;
    };
    
    class Texture2D : public Texture
    {
    public:
        static std::shared_ptr<Texture2D> Create(const std::string& filePath, const Texture2DSettings& settings = {});
        static std::shared_ptr<Texture2D> Create(const uint32_t width, const uint32_t height, const Texture2DSettings& settings = {});
        Texture2D(const std::string& filePath, const Texture2DSettings& settings = {});
        Texture2D(const uint32_t width, const uint32_t height, const Texture2DSettings& settings = {});
        ~Texture2D();

        void SetData(const void* data, uint32_t size);
        
        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        uint32_t GetTextureID() const override { return m_TextureID; }
        
        void Bind(uint32_t slot = 0) const override;

    private:
        std::string m_FilePath;
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_TextureID;
        InternalFormat m_InternalFormat;
        DataFormat m_DataFormat;
    };
}