#include "Renderer2D.h"
#include "RenderSystem2D.h"
#include "RenderSystemMap.h"

namespace Nit::Render::Renderer2D
{
    using TextureArray = DynamicArray<Texture2DPtr>;
    using SlotArray    = DynamicArray<int32_t>;
    
    constexpr uint32_t         MaxTextureSlots = 32;
    APIPtr                     RenderAPI       = nullptr;
    UniquePtr<RenderSystemMap> RenderSystems   = nullptr;
    UniquePtr<Resources>       RenderResources = nullptr;
    TextureArray               TexturesToBind  = TextureArray(MaxTextureSlots);
    SlotArray                  TextureSlots    = SlotArray(MaxTextureSlots);
    uint32_t                   LastTextureSlot = 1;
    WeakShaderPtr              LastShader;
    String                     LastPrimitiveID;
    
    void Init(GraphicsAPI api)
    {
        RenderAPI = API::Create(api);
        RenderSystems = CreateUniquePtr<RenderSystemMap>(RenderAPI, &NextBatch);
        RenderResources = CreateUniquePtr<Resources>(RenderAPI);
        TexturesToBind[0] = RenderResources->GetWhiteTexture2D().lock();
    }

    void Invalidate()
    {
        for (uint32_t i = 0; i < LastTextureSlot; i++)
            TexturesToBind[i]->Bind(i);
        RenderSystems->Each([](const String&, IRenderSystem2D& renderSystem2D) { renderSystem2D.Draw(); });
    }
    
    void StartBatch()
    {
        RenderSystems->Each([](const String&, IRenderSystem2D& renderSystem2D) { renderSystem2D.ResetBatch(); });
        LastTextureSlot = 1;
    }
    
    void NextBatch()
    {
        Invalidate();
        StartBatch();
    }

    Resources& GetResources()
    {
        assert(RenderResources);
        return *RenderResources.get();
    }

    RenderSystemMap& GetRenderSystemMap()
    {
        assert(RenderSystems);
        return *RenderSystems.get();
    }

    APIPtr GetRenderAPI()
    {
        return RenderAPI;
    }

    void BindPrimitive(const Primitive2D& primitive)
    {
        const String primtiveID = primitive.GetID();
        if (LastPrimitiveID.empty() || primtiveID == LastPrimitiveID)
        {
            return;
        }
        NextBatch();
        LastPrimitiveID = primtiveID;
    }

    int CalculateTextureSlot(const Texture2DPtr& texture)
    {
        int textureSlot = 0;

        if (texture)
        {
            // Search existing texture
            for (uint32_t i = 1; i < LastTextureSlot; i++)
            {
                if (TexturesToBind[i]->GetTextureID() == texture->GetTextureID())
                {
                    textureSlot = i;
                    break;
                }
            }

            // If not exists save the new texture
            if (textureSlot == 0)
            {
                if (LastTextureSlot > MaxTextureSlots)
                {
                    NextBatch();
                }

                TexturesToBind[LastTextureSlot] = texture;
                textureSlot = LastTextureSlot;
                LastTextureSlot++;
            }
        }
        return textureSlot;
    }

    void BindShader(const ShaderPtr& shader)
    {
        if (!shader)
        {
            assert(false && "Missing shader!");
        }
        
        if (shader)
        {
            ShaderPtr lastShader = LastShader.lock();
            
            if (shader && shader != lastShader)
            {
                NextBatch();
                shader->Bind();
                lastShader = shader;
            }
            return;
        }
    }
}