#include "Render2DState.h"

namespace Nit
{
    CRender2DState::CRender2DState(const TFunctionPtr<void()>& nextBatchFn, const TWeakTexture2DPtr& whiteTexture)
        : m_NextBatchFn(nextBatchFn)
    {
        m_TexturesToBind[0] = whiteTexture.lock();
    }
    
    void CRender2DState::NextBatch()
    {
        assert(m_NextBatchFn);   
        m_NextBatchFn();
    }
    
    void CRender2DState::ResetTextureBindings()
    {
        m_LastTextureSlot = 1;
    }

    void CRender2DState::BindPrimitive(const CPrimitive2D& primitive)
    {
        const TString primitiveID = primitive.GetID();
        if (m_LastPrimitiveID.empty() || primitiveID == m_LastPrimitiveID)
        {
            return;
        }
        m_NextBatchFn();
        m_LastPrimitiveID = primitiveID;
    }
    
    int CRender2DState::BindTexture(const TTexture2DPtr& texture)
    {
        int textureSlot = 0;

        if (texture)
        {
            // Search existing texture
            for (uint32_t i = 1; i < m_LastTextureSlot; i++)
            {
                if (m_TexturesToBind[i]->GetTextureID() == texture->GetTextureID())
                {
                    textureSlot = i;
                    break;
                }
            }

            // If not exists save the new texture
            if (textureSlot == 0)
            {
                if (m_LastTextureSlot > MaxTextureSlots)
                {
                    m_NextBatchFn();
                }

                m_TexturesToBind[m_LastTextureSlot] = texture;
                textureSlot = m_LastTextureSlot;
                m_LastTextureSlot++;
            }
        }
        return textureSlot;
    }

    void CRender2DState::BindShader(const TShaderPtr& shader)
    {
        if (!shader)
        {
            assert(false && "Missing shader!");
        }
        
        if (shader)
        {
            TShaderPtr lastShader = m_LastShader.lock();
            
            if (shader && shader != lastShader)
            {
                m_NextBatchFn();
                shader->Bind();
                lastShader = shader;
            }
            return;
        }
    }

    void CRender2DState::BindTextures()
    {
        for (uint32_t i = 0; i < m_LastTextureSlot; i++)
            m_TexturesToBind[i]->Bind(i);
    }
}