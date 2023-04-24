#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "Core/Utility/Misc.h"

namespace Nit
{
    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& filePath, const Texture2DSettings& settings)
    {
        return std::make_shared<Texture2D>(filePath, settings);
    }

    std::shared_ptr<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height,
        const Texture2DSettings& settings)
    {
        return std::make_shared<Texture2D>(width, height, settings);
    }

    static void SetMagFilter(const uint32_t textureId, const MagFilter magFilter)
    {
        switch (magFilter)
        {
        case MagFilter::Linear:
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR); return;
        case MagFilter::Nearest:
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST); return;
        }
    }

    static void SetMinFilter(const uint32_t textureId, const MinFilter magFilter)
    {
        switch (magFilter)
        {
        case MinFilter::Linear:
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR); return;
        case MinFilter::Nearest:
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST); return;
        }
    }

    static void SetWrapMode(const uint32_t textureId, const TextureCoordinate textureCoordinate, const WrapMode wrapMode)
    {
        const GLenum coord = textureCoordinate == TextureCoordinate::U ? GL_TEXTURE_WRAP_S : GL_TEXTURE_WRAP_T;
        
        switch (wrapMode)
        {
        case WrapMode::Repeat:
            glTextureParameteri(textureId, coord, GL_REPEAT); return;
        case WrapMode::ClampToEdge:
            glTextureParameteri(textureId, coord, GL_CLAMP_TO_EDGE); return;
        }
    }

    static InternalFormat GetInternalFormatFromOpenGl(GLenum internalFormat)
    {
        return InternalFormat::RGB8;
    }
    
    // static GLenum GetOpenGlInternalFormat(InternalFormat)
    // {
    //     return GL_RGB8;
    // }

    static GLenum GetOpenGlDataFormat(const DataFormat dataFormat)
    {
        switch (dataFormat)
        {
        case DataFormat::RGB:
            return GL_RGB;
        case DataFormat::RGBA:
            return GL_RGBA;
        case DataFormat::None:
            return 0;
        }
        return 0;
    }

    static DataFormat GetDataFormatFromOpenGl(const GLenum dataFormat)
    {
        switch (dataFormat)
        {
        case GL_RGBA:
            return DataFormat::RGBA;
        case GL_RGB:
            return DataFormat::RGB;
        }
        return DataFormat::None;
    }
    
    Texture2D::Texture2D(const std::string& filePath, const Texture2DSettings& settings)
        : m_FilePath(filePath)
          , m_Width(0)
          , m_Height(0)
          , m_TextureID(0)
          , m_InternalFormat(InternalFormat::None)
          , m_DataFormat(DataFormat::None)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        const std::string dir = CurrentDirectory() + "/" + filePath;
        stbi_uc* data = stbi_load(dir.c_str(), &width, &height, &channels, 0);
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = GetInternalFormatFromOpenGl(internalFormat);
        m_DataFormat = GetDataFormatFromOpenGl(dataFormat);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

        SetMinFilter(m_TextureID, settings.MinFilter);
        SetMagFilter(m_TextureID, settings.MagFilter);

        SetWrapMode(m_TextureID, TextureCoordinate::U, settings.WrapModeU);
        SetWrapMode(m_TextureID, TextureCoordinate::V, settings.WrapModeV);

        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    Texture2D::Texture2D(const uint32_t width, const uint32_t height, const Texture2DSettings& settings)
        : m_Width(width)
        , m_Height(height)
        , m_TextureID(0)
        , m_InternalFormat(InternalFormat::None)
        , m_DataFormat(DataFormat::None)
    {
        constexpr GLenum internalFormat = GL_RGB8;
        constexpr GLenum dataFormat = GL_RGBA;
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);
        
        SetMinFilter(m_TextureID, settings.MinFilter);
        SetMagFilter(m_TextureID, settings.MagFilter);
        
        SetWrapMode(m_TextureID, TextureCoordinate::U, settings.WrapModeU);
        SetWrapMode(m_TextureID, TextureCoordinate::V, settings.WrapModeV);

        m_InternalFormat = GetInternalFormatFromOpenGl(internalFormat);
        m_DataFormat = GetDataFormatFromOpenGl(dataFormat);
    }
    
    void Texture2D::SetData(const void* data, const uint32_t size)
    {
        const uint32_t bytesPerChannel = m_DataFormat == DataFormat::RGBA ? 4 : 3;
        assert(size == m_Width * m_Height * bytesPerChannel);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GetOpenGlDataFormat(m_DataFormat),
            GL_UNSIGNED_BYTE, data);
    }
    
    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_TextureID);
    }
    
    void Texture2D::Bind(const uint32_t slot) const
    {
        glBindTextureUnit(slot, m_TextureID);
    }
}
