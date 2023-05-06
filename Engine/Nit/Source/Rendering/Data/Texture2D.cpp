#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "Core/Asset/Asset.h"

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;

    registration::enumeration<MinFilter>("MinFilter")
    (
        value("Linear", MinFilter::Linear),
        value("Nearest", MinFilter::Nearest)
    );
    registration::enumeration<MagFilter>("MagFilter")
    (
        value("Linear", MagFilter::Linear),
        value("Nearest", MagFilter::Nearest)
    );
    registration::enumeration<WrapMode>("WrapMode")
    (
        value("Repeat", WrapMode::Repeat),
        value("ClampToEdge", WrapMode::ClampToEdge)
    );

    registration::class_<Texture2DSettings>("Texture2DSettings")
        .constructor<>()
        .property("CreateFromFile", &Texture2DSettings::CreateFromFile)
        .property("Width", &Texture2DSettings::Width)
        .property("Height", &Texture2DSettings::Height)
        .property("MagFilter", &Texture2DSettings::MagFilter)
        .property("MinFilter", &Texture2DSettings::MinFilter)
        .property("WrapModeU", &Texture2DSettings::WrapModeU)
        .property("WrapModeV", &Texture2DSettings::WrapModeV);

    registration::class_<Texture2D>("Texture2D")
        .constructor<const std::string&, const std::string&, const Id&>()
        .property("Texture2DSettings", &Texture2D::m_Settings);
}

namespace Nit
{
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

    static InternalFormat GetInternalFormatFromOpenGl(const GLenum internalFormat)
    {
        switch (internalFormat)
        {
        case GL_RGB8:
            return InternalFormat::RGB8;
        case GL_RGBA8:
            return InternalFormat::RGBA8;
        }
        return InternalFormat::RGB8;
    }
    
    static GLenum GetOpenGlInternalFormat(const InternalFormat internalFormat)
    {
        switch (internalFormat) {
            case InternalFormat::None: return 0;
            case InternalFormat::RGB8: return GL_RGB8;
            case InternalFormat::RGBA8: return GL_RGBA8;
        default: return 0;
        }
    }

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
    
    Texture2D::Texture2D(const std::string& name, const std::string& path, const Id& id)
          : Nit::Asset(name, path, id)
    {
    }
    
    void Texture2D::SetData(const void* data, const uint32_t size)
    {
        const uint32_t bytesPerChannel = m_DataFormat == DataFormat::RGBA ? 4 : 3;
        assert(size == GetWidth() * GetHeight() * bytesPerChannel);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, GetWidth(), GetHeight(),
            GetOpenGlDataFormat(m_DataFormat),
            GL_UNSIGNED_BYTE, data);
    }

    bool Texture2D::Load()
    {
        const std::string absolutePath = GetAbsolutePath();
        if (absolutePath == "None") return true;

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        m_Data = stbi_load(GetAbsolutePath().c_str(), &width, &height, &channels, 0);

        if (!m_Data)
            return false;

        m_Width = width;
        m_Height = height;
        m_Channels = channels;
        
        return true;
    }

    void Texture2D::Initialize()
    {
        UploadToGPU();
    }

    void Texture2D::UploadToGPU()
    {
        if (m_bUploaded) return;
        if (m_Settings.CreateFromFile)
        {
            GLenum internalFormat = 0, dataFormat = 0;
            if (m_Channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (m_Channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            m_InternalFormat = GetInternalFormatFromOpenGl(internalFormat);
            m_DataFormat = GetDataFormatFromOpenGl(dataFormat);

            glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
            glTextureStorage2D(m_TextureID, 1, internalFormat, GetWidth(), GetHeight());
            
            SetMinFilter(m_TextureID, m_Settings.MinFilter);
            SetMagFilter(m_TextureID, m_Settings.MagFilter);

            SetWrapMode(m_TextureID, TextureCoordinate::U, m_Settings.WrapModeU);
            SetWrapMode(m_TextureID, TextureCoordinate::V, m_Settings.WrapModeV);

            glTextureSubImage2D(m_TextureID, 0, 0, 0, GetWidth(), GetHeight(),
                dataFormat, GL_UNSIGNED_BYTE, m_Data);
            stbi_image_free(m_Data);
            m_bUploaded = true;
            return;
        }

        m_Width = m_Settings.Width;
        m_Height = m_Settings.Height;
        
        constexpr GLenum internalFormat = GL_RGBA8;
        constexpr GLenum dataFormat = GL_RGBA;
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, internalFormat, GetWidth(), GetHeight());
        
        SetMinFilter(m_TextureID, m_Settings.MinFilter);
        SetMagFilter(m_TextureID, m_Settings.MagFilter);
        
        SetWrapMode(m_TextureID, TextureCoordinate::U, m_Settings.WrapModeU);
        SetWrapMode(m_TextureID, TextureCoordinate::V, m_Settings.WrapModeV);

        m_InternalFormat = GetInternalFormatFromOpenGl(internalFormat);
        m_DataFormat = GetDataFormatFromOpenGl(dataFormat);
        m_bUploaded = true;
    }

    bool Texture2D::Unload()
    {
        glDeleteTextures(1, &m_TextureID);
        return true;
    }
    
    void Texture2D::Bind(const uint32_t slot) const
    {
        glBindTextureUnit(slot, m_TextureID);
    }
}