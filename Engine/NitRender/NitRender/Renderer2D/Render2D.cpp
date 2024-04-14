#include "Render2D.h"
#include "Renderer2D.h"

namespace Nit::Render2D
{
    using TTextureArray = TDynamicArray<TTexture2DPtr>;
    using TSlotArray    = TDynamicArray<int32_t>;
    
    TMap<TString, IRenderer2D*>     Renderers = {};
    TRenderAPIPtr                   RenderAPI       = nullptr;
    TUniquePtr<CRenderResources>    RenderResources = nullptr;
    TSharedPtr<CRender2DState>      Render2DState   = nullptr;
    
    void AddRenderer(const TString& rendererID, IRenderer2D* renderer, const TShaderPtr& defaultShader)
    {
        renderer->Init(RenderAPI, Render2DState, defaultShader);
        Renderers[rendererID] = renderer;
    }
    
    IRenderer2D& GetRenderer(const TString& rendererID)
    {
        return *Renderers[rendererID];
    }
    
    void Init(EGraphicsAPI api)
    {
        RenderAPI         = CRenderAPI::Create(api);
        RenderResources   = CreateUniquePtr<CRenderResources>(RenderAPI);
        Render2DState     = CreateSharedPtr<CRender2DState>(&NextBatch, RenderResources->GetWhiteTexture2D());
        
        CreateRenderSystem<CSpriteRenderer>(RenderResources->DefaultSpriteShader);
    }

    void EndBatch()
    {
        Render2DState->BindTextures();

        for (auto&[id, renderer2D] : Renderers)
        {
            renderer2D->SubmitBatchToGPU();
        }
    }
    
    void BeginBatch()
    {
        for (auto&[id, renderer2D] : Renderers)
        {
            renderer2D->ResetBatch();
        }
        
        Render2DState->ResetTextureBindings();
    }
    
    void NextBatch()
    {
        EndBatch();
        BeginBatch();
    }

    CSpriteRenderer& GetSpriteRenderer()
    {
        return GetRenderer<CSpriteRenderer>("SpriteRenderSystem");
    }

    CRenderResources& GetResources()
    {
        assert(RenderResources);
        return *RenderResources.get();
    }

    TRenderAPIPtr GetRenderAPI()
    {
        return RenderAPI;
    }
} 