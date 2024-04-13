#pragma once
#include "Core/API.h"
#include "Objects/Texture2D.h"
#include "Objects/Shader.h"

namespace Nit::Render
{
    class Resources
    {
    public:
        Resources(const APIPtr& api);
        ~Resources();
        
        const WeakTexture2DPtr& GetWhiteTexture2D      () const { return m_WhiteTexture2D; }
        const WeakShaderPtr&    GetErrorShader         () const { return m_ErrorShader;  }
        
        WeakTexture2DPtr        CreateTexture2D        ();
        bool                    DestroyTexture2D       (const WeakTexture2DPtr& texture2DWeak);
        WeakShaderPtr           CreateShader           ();
        bool                    DestroyShader          (const WeakShaderPtr& shaderWeak);

        ShaderPtr DefaultSpriteShader = nullptr;
        ShaderPtr DefaultCircleShader = nullptr;
        ShaderPtr DefaultLineShader   = nullptr;
        
    private:
        DynamicArray<Texture2DPtr> m_Textures2D;
        DynamicArray<ShaderPtr>    m_Shaders;
        APIPtr                     m_API;
        Texture2DPtr               m_WhiteTexture2D = nullptr;
        ShaderPtr                  m_ErrorShader    = nullptr;
    };
}