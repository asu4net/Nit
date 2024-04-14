#pragma once
#include "NitRender/Core/RenderCommand.h"
#include "NitRender/Objects/Texture2D.h"
#include "NitRender/Objects/Shader.h"
#include "NitRender/Renderer/RenderResources.h"
#include "NitRender/Renderer2D/Renderer2D.h"

// create core project, and move there common includes, container definitions, etc...
// create randomId class in core project. Use it to create static funcs in primitives and render system, to provide an id for bind check

// keep thinking how to make renderer2D and render systems cleaner...
// change render2D to CRenderEngine2D
// make primitive and shader binding static functions in render2D or in a separated static class...
// untie renderer from CRenderEngine2D
// create the rendersystem stack

namespace Nit::Render2D
{
    void Init(EGraphicsAPI api);
    void EndBatch();
    void BeginBatch();
    void NextBatch();

    void AddRenderer(const TString& rendererID, IRenderer2D* renderer, const TShaderPtr& defaultShader);
    IRenderer2D& GetRenderer(const TString& rendererID);
    
    template<typename T>
    void CreateRenderSystem(const TShaderPtr& defaultShader)
    {
        T* system = new T();
        AddRenderer(system->GetPrimitiveID(), static_cast<IRenderer2D*>(system), defaultShader);
    }

    template<typename T>
    T& GetRenderer(const TString& renderSystemID)
    {
        return static_cast<T&>(GetRenderer(renderSystemID));
    }

    template<typename T>
    inline void SubmitPrimitive(const T& primitive2D)
    {
    }
    
    CSpriteRenderer& GetSpriteRenderer();

    template<>
    inline void SubmitPrimitive<CSpritePrimitive>(const CSpritePrimitive& primitive2D)
    {
        GetSpriteRenderer().SubmitPrimitive(primitive2D);
    }
    
    CRenderResources& GetResources();
    TRenderAPIPtr     GetRenderAPI();
    
    void BindPrimitive(const CPrimitive2D& primitive);
    int  CalculateTextureSlot(const TTexture2DPtr& texture);
    void BindShader(const TShaderPtr& shader);
}