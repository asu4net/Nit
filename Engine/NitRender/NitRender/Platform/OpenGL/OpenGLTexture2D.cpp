#include "OpenGLTexture2D.h"
#include <GLAD/glad.h>

namespace Nit
{
    static void SetMagFilter(const uint32_t textureId, const EMagnificationFilter magFilter)
    {
        switch (magFilter)
        {
        case EMagnificationFilter::Linear:
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR); return;
        case EMagnificationFilter::Nearest:
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST); return;
        }
    }

    static void SetMinFilter(const uint32_t textureId, const EMinificationFilter magFilter)
    {
        switch (magFilter)
        {
        case EMinificationFilter::Linear:
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR); return;
        case EMinificationFilter::Nearest:
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST); return;
        }
    }

    static void SetWrapMode(const uint32_t textureId, const ETextureCoordinate textureCoordinate, const ETextureWrapMode wrapMode)
    {
        const GLenum coord = textureCoordinate == ETextureCoordinate::U ? GL_TEXTURE_WRAP_S : GL_TEXTURE_WRAP_T;

        switch (wrapMode)
        {
        case ETextureWrapMode::Repeat:
            glTextureParameteri(textureId, coord, GL_REPEAT); return;
        case ETextureWrapMode::ClampToEdge:
            glTextureParameteri(textureId, coord, GL_CLAMP_TO_EDGE); return;
        }
    }

    COpenGlTexture2D::COpenGlTexture2D() = default;

    COpenGlTexture2D::~COpenGlTexture2D()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    bool COpenGlTexture2D::UploadToGPU(const CTexture2DSettings& settings, const void* data)
    {
        if (!data)
        {
            return false;
        }
        
        if (m_Uploaded)
        {
            glDeleteTextures(1, &m_TextureID);
            m_Uploaded = false;
        }

        GLenum internalFormat = 0, dataFormat = 0;

        if (settings.Channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (settings.Channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, internalFormat, settings.Width, settings.Height);

        SetMinFilter(m_TextureID, settings.MinFilter);
        SetMagFilter(m_TextureID, settings.MagFilter);

        SetWrapMode(m_TextureID, ETextureCoordinate::U, settings.WrapModeU);
        SetWrapMode(m_TextureID, ETextureCoordinate::V, settings.WrapModeV);

        glTextureSubImage2D(m_TextureID, 0, 0, 0, settings.Width, settings.Height,
            dataFormat, GL_UNSIGNED_BYTE, data);

        m_Uploaded = true;
        return true;
    }

    void COpenGlTexture2D::Bind(const uint32_t slot) const
    {
        glBindTextureUnit(slot, m_TextureID);
    }
}