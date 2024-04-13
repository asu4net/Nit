#include "SpriteRenderSystem.h"
#include "Renderer2D.h"

namespace Nit::Render
{
    void SpriteRenderSystem::SubmitPrimitive(const Primitive2D& primitive2D)
    {
        Renderer2D::BindPrimitive(primitive2D);
        const auto& sprite  = static_cast<const SpritePrimitive&>(primitive2D);
        const int textureSlot = Renderer2D::CalculateTextureSlot(sprite.TextureToBind);
        Renderer2D::BindShader(sprite.ShaderToBind ? sprite.ShaderToBind : Renderer2D::GetResources().DefaultSpriteShader);
        Array<Vector2, 4> vertexUV = sprite.VertexUVs;
        FlipQuadVertexUVs(sprite.bFlipX, sprite.bFlipY, vertexUV);
        
        for (uint32_t i = 0; i < 4; ++i)
        {
            SpriteVertex vertex;
            
            Vector3 localVertexPos = sprite.VertexPositions[i];
            vertex.LocalPosition   = localVertexPos;
            localVertexPos.x      *= sprite.Size.x;
            localVertexPos.y      *= sprite.Size.y;
            vertex.Position        = Vector4(localVertexPos, 1.f) * sprite.Transform * sprite.ProjectionView;
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

    void SpriteRenderSystem::SetBufferElements(VertexBuffer& VBO) const
    {
        VBO.SetLayout({
               {ShaderDataType::Float3, "a_Position"      },
               {ShaderDataType::Float3, "a_LocalPosition" },
               {ShaderDataType::Float4, "a_TintColor"     },
               {ShaderDataType::Float2, "a_UVCoords"      },
               {ShaderDataType::Float , "a_TextureSlot"   },
               {ShaderDataType::Float , "a_Time"          },
               {ShaderDataType::Float,  "a_EntityID"      }
           });
    }
}
