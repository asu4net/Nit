#include "Resources.h"
#include "RawShaderStrings.h"

namespace Nit::Render
{
    Resources::Resources(const APIPtr& api)
        : m_API(api)
    {
        assert(m_API && "API should be valid!");
        
        // White texture creation
        {
            Texture2DSettings settings;
            settings.Width = 1;
            settings.Height = 1;
            settings.Channels = 4;
        
            m_WhiteTexture = Texture2D::Create(m_API->GetGraphicsAPI());
            constexpr uint32_t whiteTextureData = 0xffffffff;
            m_WhiteTexture->UploadToGPU(settings, &whiteTextureData);
        }
        
        // Default sprite shader creation
        {
            const GraphicsAPI api = m_API->GetGraphicsAPI();
            DefaultSpriteShader = Shader::Create(api);
            DefaultSpriteShader->Compile(GetSpriteVertShaderStr(), GetSpriteFragShaderStr());

            DefaultCircleShader = Shader::Create(api);
            DefaultCircleShader->Compile(GetCircleVertShaderStr(), GetCircleFragShaderStr());

            DefaultLineShader = Shader::Create(api);
            DefaultLineShader->Compile(GetLineVertShaderStr  (), GetLineFragShaderStr  ());

            m_ErrorShader = Shader::Create(api);
            m_ErrorShader->Compile(GetErrorVertShaderStr (), GetErrorFragShaderStr ());
        }
    }

    Resources::~Resources()
    {
        for (ShaderPtr& shader : m_Shaders)
        {
            shader.reset();
        }
        
        for (Texture2DPtr& texture : m_Textures2D)
        {
            texture.reset();
        }

        m_Shaders           .clear();
        m_Textures2D        .clear();
        m_WhiteTexture      .reset();
        m_ErrorShader       .reset();
        DefaultSpriteShader .reset();
        DefaultCircleShader .reset();
        DefaultLineShader   .reset();
    }

    WeakTexture2DPtr Resources::CreateTexture()
    {
        Texture2DPtr texture = Texture2D::Create(m_API->GetGraphicsAPI());
        m_Textures2D.push_back(texture);
        return texture;
    }
    
    bool Resources::DestroyTexture(const WeakTexture2DPtr& texture2DWeak)
    {
        if (texture2DWeak.expired())
            return false;

        Texture2DPtr texture = texture2DWeak.lock();

        auto it = std::find(m_Textures2D.begin(), m_Textures2D.end(), texture);

        if (it == m_Textures2D.end())
            return false;

        m_Textures2D.erase(it);
        texture.reset();
        return true;
    }
    
    WeakShaderPtr Resources::CreateShader()
    {
        ShaderPtr shader = Shader::Create(m_API->GetGraphicsAPI());
        m_Shaders.push_back(shader);
        return shader;
    }
    
    bool Resources::DestroyShader(const WeakShaderPtr& shaderWeak)
    {
        if (shaderWeak.expired())
            return false;

        ShaderPtr shader = shaderWeak.lock();
        
        auto it = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
        
        if (it == m_Shaders.end())
            return false;

        m_Shaders.erase(it);
        shader.reset();
        return true;
    }
}