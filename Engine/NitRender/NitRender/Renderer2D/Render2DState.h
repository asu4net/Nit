#pragma once
#include "NitRender/Renderer2D/Primitive2D.h"
#include "NitRender/Objects/Shader.h"
#include "NitRender/Objects/Texture2D.h"

namespace Nit
{
    using TTextureArray = TDynamicArray<TTexture2DPtr>;
    using TSlotArray    = TDynamicArray<int32_t>;
    
    class CRender2DState
    {
    public:
        CRender2DState(const TFunctionPtr<void()>& nextBatchFn, const TWeakTexture2DPtr& whiteTexture);
        ~CRender2DState() = default;
        
        void NextBatch();
        void ResetTextureBindings();
        void BindPrimitive(const CPrimitive2D& primitive);
        int  BindTexture(const TTexture2DPtr& texture);
        void BindShader(const TShaderPtr& shader);
        void BindTextures();
        
        inline static constexpr uint32_t  MaxTextureSlots = 32;

    private:
        TFunctionPtr<void()> m_NextBatchFn     = nullptr;
        TTextureArray        m_TexturesToBind  = TTextureArray(MaxTextureSlots);
        TSlotArray           m_TextureSlots    = TSlotArray(MaxTextureSlots);
        uint32_t             m_LastTextureSlot = 1;
        TWeakShaderPtr       m_LastShader      = {};
        TString              m_LastPrimitiveID = "";
    };

    using TRender2DStatePtr = TSharedPtr<CRender2DState>;
}
