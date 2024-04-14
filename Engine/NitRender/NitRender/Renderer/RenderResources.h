#pragma once
#include "NitRender/Core/RenderAPI.h"
#include "NitRender/Objects/Texture2D.h"
#include "NitRender/Objects/Shader.h"

namespace Nit
{
    class CRenderResources
    {
    public:
        CRenderResources(const TRenderAPIPtr& api);
        ~CRenderResources();
        
        TWeakTexture2DPtr GetWhiteTexture2D      () const { return m_WhiteTexture2D; }
        TWeakShaderPtr    GetErrorShader         () const { return m_ErrorShader;  }
        
        TWeakTexture2DPtr       CreateTexture2D        ();
        bool                    DestroyTexture2D       (const TWeakTexture2DPtr& texture2DWeak);
        TWeakShaderPtr          CreateShader           ();
        bool                    DestroyShader          (const TWeakShaderPtr& shaderWeak);

        TShaderPtr DefaultSpriteShader = nullptr;
        TShaderPtr DefaultCircleShader = nullptr;
        TShaderPtr DefaultLineShader   = nullptr;
        
    private:
        TDynamicArray<TTexture2DPtr> m_Textures2D     = {};
        TDynamicArray<TShaderPtr>    m_Shaders        = {};
        TRenderAPIPtr                m_API            = nullptr;
        TTexture2DPtr                m_WhiteTexture2D = nullptr;
        TShaderPtr                   m_ErrorShader    = nullptr;
    };
}