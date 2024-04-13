#include "OpenGLTexture2D.h"
#include <GLAD/glad.h>

namespace Nit::Render
{
    static void SetMagFilter(const uint32_t textureId, const MagnificationFilter magFilter)
    {
        switch (magFilter)
        {
        case MagnificationFilter::Linear:
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR); return;
        case MagnificationFilter::Nearest:
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST); return;
        }
    }

    static void SetMinFilter(const uint32_t textureId, const MinificationFilter magFilter)
    {
        switch (magFilter)
        {
        case MinificationFilter::Linear:
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR); return;
        case MinificationFilter::Nearest:
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST); return;
        }
    }

    static void SetWrapMode(const uint32_t textureId, const TextureCoordinate textureCoordinate, const TextureWrapMode wrapMode)
    {
        const GLenum coord = textureCoordinate == TextureCoordinate::U ? GL_TEXTURE_WRAP_S : GL_TEXTURE_WRAP_T;

        switch (wrapMode)
        {
        case TextureWrapMode::Repeat:
            glTextureParameteri(textureId, coord, GL_REPEAT); return;
        case TextureWrapMode::ClampToEdge:
            glTextureParameteri(textureId, coord, GL_CLAMP_TO_EDGE); return;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D() = default;

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    bool OpenGLTexture2D::UploadToGPU(const Texture2DSettings& settings, const void* data)
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

        SetWrapMode(m_TextureID, TextureCoordinate::U, settings.WrapModeU);
        SetWrapMode(m_TextureID, TextureCoordinate::V, settings.WrapModeV);

        glTextureSubImage2D(m_TextureID, 0, 0, 0, settings.Width, settings.Height,
            dataFormat, GL_UNSIGNED_BYTE, data);

        m_Uploaded = true;
        return true;
    }

    void OpenGLTexture2D::Bind(const uint32_t slot) const
    {
        glBindTextureUnit(slot, m_TextureID);
    }
}