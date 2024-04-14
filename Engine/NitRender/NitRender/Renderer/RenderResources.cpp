#include "RenderResources.h"
#include "RawShaderStrings.h"

namespace Nit
{
    CRenderResources::CRenderResources(const TRenderAPIPtr& api)
        : m_API(api)
    {
        assert(m_API && "API should be valid!");
        
        // White texture creation
        {
            CTexture2DSettings settings;
            settings.Width = 1;
            settings.Height = 1;
            settings.Channels = 4;
        
            m_WhiteTexture2D = Texture2D::Create(m_API->GetGraphicsAPI());
            constexpr uint32_t whiteTextureData = 0xffffffff;
            m_WhiteTexture2D->UploadToGPU(settings, &whiteTextureData);
        }
        
        // Default sprite shader creation
        {
            const EGraphicsAPI api = m_API->GetGraphicsAPI();
            DefaultSpriteShader = CShader::Create(api);
            DefaultSpriteShader->Compile(GetSpriteVertShaderStr(), GetSpriteFragShaderStr());

            DefaultCircleShader = CShader::Create(api);
            DefaultCircleShader->Compile(GetCircleVertShaderStr(), GetCircleFragShaderStr());

            DefaultLineShader = CShader::Create(api);
            DefaultLineShader->Compile(GetLineVertShaderStr  (), GetLineFragShaderStr  ());

            m_ErrorShader = CShader::Create(api);
            m_ErrorShader->Compile(GetErrorVertShaderStr (), GetErrorFragShaderStr ());
        }
    }

    CRenderResources::~CRenderResources()
    {
        for (TShaderPtr& shader : m_Shaders)
        {
            shader.reset();
        }
        
        for (TTexture2DPtr& texture : m_Textures2D)
        {
            texture.reset();
        }

        m_Shaders           .clear();
        m_Textures2D        .clear();
        m_WhiteTexture2D      .reset();
        m_ErrorShader       .reset();
        DefaultSpriteShader .reset();
        DefaultCircleShader .reset();
        DefaultLineShader   .reset();
    }

    TWeakTexture2DPtr CRenderResources::CreateTexture2D()
    {
        TTexture2DPtr texture = Texture2D::Create(m_API->GetGraphicsAPI());
        m_Textures2D.push_back(texture);
        return texture;
    }
    
    bool CRenderResources::DestroyTexture2D(const TWeakTexture2DPtr& texture2DWeak)
    {
        if (texture2DWeak.expired())
            return false;

        TTexture2DPtr texture = texture2DWeak.lock();

        auto it = std::find(m_Textures2D.begin(), m_Textures2D.end(), texture);

        if (it == m_Textures2D.end())
            return false;

        m_Textures2D.erase(it);
        texture.reset();
        return true;
    }
    
    TWeakShaderPtr CRenderResources::CreateShader()
    {
        TShaderPtr shader = CShader::Create(m_API->GetGraphicsAPI());
        m_Shaders.push_back(shader);
        return shader;
    }
    
    bool CRenderResources::DestroyShader(const TWeakShaderPtr& shaderWeak)
    {
        if (shaderWeak.expired())
            return false;

        TShaderPtr shader = shaderWeak.lock();
        
        auto it = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
        
        if (it == m_Shaders.end())
            return false;

        m_Shaders.erase(it);
        shader.reset();
        return true;
    }
}