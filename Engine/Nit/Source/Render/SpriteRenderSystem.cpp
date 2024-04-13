#include "NitPCH.h"
#include "SpriteRenderSystem.h"
#include "RenderComponents.h"
#include "Renderer.h"

namespace Nit
{
    const ShaderPtr& SpriteRenderSystem::GetDefaultShader() const
    {
        return Renderer::GetSpriteShader();
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
    
    void SpriteRenderSystem::SubmitVertices()
    {
        auto view = Renderer::GetRegistry().view<RenderComponent, Primitive2DComponent, SpriteShapeComponent>();
        for (RawEntity rawEntity : view)
        {
            auto[baseRenderData, primitive, sprite] = view.get<RenderComponent, Primitive2DComponent, SpriteShapeComponent>(rawEntity);

            RenderEntity entity = rawEntity;
            
            SubmitShader(baseRenderData.ShaderID);
            
            uint32_t textureSlot = 0;

            if (auto texture = Renderer::GetTexture2D(sprite.TextureID))
            {
                textureSlot = Renderer::CalculateTextureSlot(texture);
            }

            Array<Vector2, 4> vertexUV = primitive.VertexUVs;
            Render::FlipQuadVertexUVs(sprite.bFlipX, sprite.bFlipY, vertexUV);

            const float appTime = Time::GetApplicationTime();
            const Matrix4 projectionViewMatrix = Renderer::GetProjectionViewMatrix(); //this could data
            
            for (uint32_t i = 0; i < 4; i++)
            {
                SpriteVertex vertex;
                Vector3 localVertexPos = primitive.VertexPositions[i];
                vertex.LocalPosition = localVertexPos;
                localVertexPos.x *= sprite.Size.x;
                localVertexPos.y *= sprite.Size.y;
                vertex.Position = Vector4(localVertexPos, 1.f) * baseRenderData.Transform * projectionViewMatrix;
                vertex.Time = appTime;
                vertex.UVCoords = vertexUV[i];
                vertex.UVCoords.x *= sprite.UVScale.y;
                vertex.UVCoords.y *= sprite.UVScale.x;
                vertex.TintColor = primitive.TintColor;
                vertex.EntityID = baseRenderData.EntityID;
                vertex.TextureSlot = textureSlot;
                
                SubmitVertex(vertex);
            }

        }
    }
}