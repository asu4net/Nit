#include "RenderSystem2D.h"
#include "IndexBuffer.h"
#include "RenderCommand.h"
#include "Renderer.h"
#include "RendererShader.h"
#include "RendererTexture2D.h"

namespace Nit
{
    namespace InternalRenderSystem2D
    {
        constexpr uint32_t  MaxPrimitives        = 3000;
        constexpr uint32_t  VerticesPerPrimitive = 4;
        constexpr uint32_t  IndicesPerPrimitive  = 6;

        SharedPtr<IndexBuffer> CreateQuadIndexBuffer(uint32_t maxPrimitives)
        {
            const uint32_t maxIndices = maxPrimitives * IndicesPerPrimitive;

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
            const GraphicsAPI API = Renderer::GetAPI()->GetGraphicsAPI();
            auto idxBuffer = IndexBuffer::Create(API, indices, maxIndices);
            delete[] indices;
            return idxBuffer;
        }
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::OnInit() {}

    template <typename Vertex>
    void RenderSystem2D<Vertex>::OnFinish() {}

    template <typename Vertex>
    void RenderSystem2D<Vertex>::OnPreDraw() {}

    template <typename Vertex>
    uint32_t RenderSystem2D<Vertex>::GetMaxPrimitives() const { return InternalRenderSystem2D::MaxPrimitives; }
    
    template <typename Vertex>
    void RenderSystem2D<Vertex>::Init()
    {
        if (bInitialized)
        {
            return;
        }

        bInitialized = true;
        MaxVertices = GetMaxPrimitives() * InternalRenderSystem2D::VerticesPerPrimitive;
        MaxIndices  = GetMaxPrimitives() * InternalRenderSystem2D::IndicesPerPrimitive;
        IndexCount  = 0;
        Vertices    = new Vertex[MaxVertices];
        LastVertex  = Vertices;

        const GraphicsAPI API = Renderer::GetAPI()->GetGraphicsAPI();
        VAO = VertexArray::Create(API);
        VBO = VertexBuffer::Create(API, MaxVertices * sizeof(Vertex));

        SetBufferElements(VAO);

        VAO->AddVertexBuffer(VBO);
        IBO = InternalRenderSystem2D::CreateQuadIndexBuffer(GetMaxPrimitives());
        VAO->SetIndexBuffer(IBO);
        OnInit();
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::Finish()
    {
        if (!bInitialized)
        {
            return;
        }

        OnFinish();
        bInitialized = false;
        MaxVertices = 0;
        MaxIndices  = 0;
        IndexCount  = 0;
        delete[] Vertices;
        Vertices    = nullptr;
        LastVertex  = nullptr;
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::BatchVertices()
    {
        //check if vertices are correct
        SubmitVertices();
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::Draw()
    {
        if (const uint32_t vertexDataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(LastVertex) -
                reinterpret_cast<uint8_t*>(Vertices)))
        {
            VBO->SetData(Vertices, vertexDataSize);

            OnPreDraw();

            RenderCommandQueue::Submit<DrawElementsCommand>(Renderer::GetAPI(), VAO, IndexCount);
        }

        while (!RenderCommandQueue::IsEmpty())
        {
            RenderCommandQueue::ExecuteNext();
        }
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::SubmitVertex(Vertex& vertex)
    {
        *LastVertex = vertex;
        LastVertex++;
        CurrentIndex++;
        
        if (CurrentIndex == InternalRenderSystem2D::VerticesPerPrimitive)
        {
            CurrentIndex = 0; 
        }

        IndexCount += InternalRenderSystem2D::IndicesPerPrimitive;
        
        if (IndexCount + InternalRenderSystem2D::IndicesPerPrimitive >= MaxIndices)
        {
            Renderer::NextBatch();
        }
    }
}