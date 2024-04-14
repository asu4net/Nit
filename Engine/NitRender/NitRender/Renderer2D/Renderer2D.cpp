#include "Renderer2D.h"

namespace Nit
{
    void CSpriteRenderer::SubmitPrimitive(const CPrimitive2D& primitive2D)
    {
        const TRender2DStatePtr& render2DState = GetRender2DState();
        render2DState->BindPrimitive(primitive2D);
        const auto& sprite  = static_cast<const CSpritePrimitive&>(primitive2D);
        const int textureSlot = render2DState->BindTexture(sprite.TextureToBind);
        render2DState->BindShader(sprite.ShaderToBind ? sprite.ShaderToBind : GetDefaultShader());
        TArray<CVector2, 4> vertexUV = sprite.VertexUVs;
        FlipQuadVertexUVs(sprite.bFlipX, sprite.bFlipY, vertexUV);
        
        for (uint32_t i = 0; i < 4; ++i)
        {
            CSpriteVertex vertex;
            
            CVector3 localVertexPos = sprite.VertexPositions[i];
            vertex.LocalPosition   = localVertexPos;
            localVertexPos.x      *= sprite.Size.x;
            localVertexPos.y      *= sprite.Size.y;
            vertex.Position        = CVector4(localVertexPos, 1.f) * sprite.Transform * sprite.ProjectionView;
            vertex.Time            = sprite.Time;
            vertex.UVCoords        = vertexUV[i];
            vertex.UVCoords.x     *= sprite.UVScale.y;
            vertex.UVCoords.y     *= sprite.UVScale.x;
            vertex.TintColor       = sprite.TintColor;
            vertex.EntityID        = sprite.EntityID;
            vertex.TextureSlot     = textureSlot; 
            SubmitVertex(vertex);
        }
    }

    void CSpriteRenderer::SetBufferElements(CVertexBuffer& VBO) const
    {
        VBO.SetLayout({
               {EShaderDataType::Float3, "a_Position"      },
               {EShaderDataType::Float3, "a_LocalPosition" },
               {EShaderDataType::Float4, "a_TintColor"     },
               {EShaderDataType::Float2, "a_UVCoords"      },
               {EShaderDataType::Float , "a_TextureSlot"   },
               {EShaderDataType::Float , "a_Time"          },
               {EShaderDataType::Float,  "a_EntityID"      }
           });
    }
}