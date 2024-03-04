#include "SpriteBatchRenderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RendererTexture2D.h"
#include "RendererShader.h"
#include "RenderCommand.h"

namespace Nit
{
    static constexpr uint32_t s_VerticesPerSprite = 4;
    static constexpr uint32_t s_IndicesPerSprite = 6;

    void SpriteBatchRenderer::Init(const SharedPtr<RendererAPI> API)
    {
        if (m_Vertices)
        {
            delete[] m_Vertices;
            m_Vertices = nullptr;
        }

        m_RendererAPI = API;
        m_MaxVertices = MaxSprites * s_VerticesPerSprite;
        m_Vertices = new SpriteVertex[m_MaxVertices];
        m_LastVertex = m_Vertices;
        m_Textures.resize(MaxTextureSlots);

        CreateWhiteTexture();
        CreateTextureSlots();
        CreateRenderObjects();
    }

    void SpriteBatchRenderer::Begin(RenderData& renderData)
    {
        renderData.SpriteShader->Bind();
        renderData.SpriteShader->SetUniformMat4("u_ProjectionViewMatrix", renderData.ProjectionViewMat4);
        renderData.SpriteShader->SetUniformIntArray("u_TextureSlots", &m_TextureSlots.front(), MaxTextureSlots);

        StartBatch();
    }

    void SpriteBatchRenderer::SubmitSpriteVertexData(const Array<SpriteVertex, 4>& spriteVertices)
    {
        for (const SpriteVertex& spriteVertex : spriteVertices)
        {
            *m_LastVertex = spriteVertex;
            m_LastVertex++;
        }

        m_IndexCount += 6;
        m_SpriteCount++;
    }

    void SpriteBatchRenderer::End()
    {
        Flush();
    }

    void SpriteBatchRenderer::StartBatch()
    {
        m_LastVertex = m_Vertices;
        m_SpriteCount = 0;
        m_IndexCount = 0;
        m_LastTextureSlot = 1;
    }

    void SpriteBatchRenderer::Flush()
    {
        if (const uint32_t vertexDataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(m_LastVertex) -
            reinterpret_cast<uint8_t*>(m_Vertices)))
        {
            m_VertexBuffer->SetData(m_Vertices, vertexDataSize);

            for (uint32_t i = 0; i < m_LastTextureSlot; i++)
                m_Textures[i]->Bind(i);

            RenderCommandQueue::Submit<DrawElementsCommand>(m_RendererAPI, m_VertexArray, m_IndexCount);
        }

        while (!RenderCommandQueue::IsEmpty())
        {
            RenderCommandQueue::ExecuteNext();
        }
    }

    void SpriteBatchRenderer::CreateWhiteTexture()
    {
        Texture2DSettings settings;
        settings.Width = 1;
        settings.Height = 1;
        settings.Channels = 4;
        
        m_WhiteTexture = RendererTexture2D::Create(m_RendererAPI->GetGraphicsAPI());
        constexpr uint32_t whiteTextureData = 0xffffffff;
        m_WhiteTexture->UploadToGPU(settings, &whiteTextureData);
        m_Textures[0] = m_WhiteTexture;
    }

    void SpriteBatchRenderer::CreateTextureSlots()
    {
        DynamicArray<int32_t>* slots = const_cast<DynamicArray<int32_t>*>(&m_TextureSlots);
        slots->reserve(MaxTextureSlots);
        for (uint32_t i = 0; i < MaxTextureSlots; i++)
            slots->push_back(i);
    }

    void SpriteBatchRenderer::CreateRenderObjects()
    {
        const GraphicsAPI API = m_RendererAPI->GetGraphicsAPI();

        m_VertexArray = VertexArray::Create(API);
        m_VertexBuffer = VertexBuffer::Create(API, m_MaxVertices * sizeof(SpriteVertex));

        m_VertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"      },
            {ShaderDataType::Float4, "a_Color"         },
            {ShaderDataType::Float2, "a_UV"            },
            {ShaderDataType::Float , "a_TextureSlot"   },
            {ShaderDataType::Float , "a_Shape"         },
            {ShaderDataType::Float3, "a_LocalPosition" },
            {ShaderDataType::Float , "a_Thickness"     },
            {ShaderDataType::Float , "a_Fade"          },
            {ShaderDataType::Float2, "a_Bounds"        },
            {ShaderDataType::Float4, "a_RectColor"     },
            {ShaderDataType::Float,  "a_EntityID"      }
        });

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        const uint32_t maxIndices = MaxSprites * s_IndicesPerSprite;

        uint32_t* indices = new uint32_t[maxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < maxIndices; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        m_IndexBuffer = IndexBuffer::Create(API, indices, maxIndices);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        delete[] indices;
    }

    int SpriteBatchRenderer::CalculateTextureSlot(const SharedPtr<RendererTexture2D>& texture)
    {
        int textureSlot = 0;

        if (texture)
        {
            // Search existing texture
            for (uint32_t i = 1; i < m_LastTextureSlot; i++)
            {
                if (m_Textures[i]->GetTextureID() == texture->GetTextureID())
                {
                    textureSlot = i;
                    break;
                }
            }

            // If not exists save the new texture
            if (textureSlot == 0)
            {
                m_Textures[m_LastTextureSlot] = texture;
                textureSlot = m_LastTextureSlot;
                m_LastTextureSlot++;
            }
        }

        return textureSlot;
    }
}