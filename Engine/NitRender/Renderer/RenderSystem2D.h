#pragma once
#include "NitRender/Core/Command.h"
#include "NitRender/Core/API.h"
#include "NitRender/Objects/VertexArray.h"
#include "NitRender/Objects/VertexBuffer.h"
#include "NitRender/Objects/IndexBuffer.h"
#include "NitRender/Objects/Shader.h"

#define NIT_RENDER_SYSTEM(_TYPE) \
       public:  \
       const String&    GetPrimitiveID()   const override { return #_TYPE; }; \
       private: 

namespace Nit::Render
{
    struct Primitive2D
    {
        virtual String GetID() const = 0;
        
        bool              bIsVisible      = false;
        Matrix4           Transform;      // Identity
        Matrix4           ProjectionView; // Identity
        float             Time            = 0.f;
        Color             TintColor       = Color::White;
        Array<Vector3, 4> VertexPositions = GetQuadVertexPositions();
        Array<Vector2, 4> VertexUVs       = GetQuadVertexUVs();
        ShaderPtr         ShaderToBind    = nullptr;
        int               EntityID        = -1;
        int               SortingLayer    =  0;
    };
    
    class IRenderSystem2D
    {
    public:
        virtual String    GetPrimitiveID()    const = 0;
        
        virtual void Init(const APIPtr& api, const FunctionPtr<void()>& nextBatchFn)   = 0;
        virtual void Finish()                                                          = 0;
        virtual void SubmitPrimitive(const Primitive2D& primitive2D)                   = 0;
        virtual void ResetBatch()                                                      = 0;
        virtual void Draw()                                                            = 0;
    };
    
    template<typename Vertex>
    class RenderSystem2D : public IRenderSystem2D
    {
    public:
        static constexpr uint32_t  MaxPrimitives        = 3000;
        static constexpr uint32_t  VerticesPerPrimitive = 4;
        static constexpr uint32_t  IndicesPerPrimitive  = 6;
        
        RenderSystem2D() = default;
        
        virtual ~RenderSystem2D() = default;
        
        virtual uint32_t GetMaxPrimitives() const { return MaxPrimitives; }
        
        virtual void OnInit() {}
        virtual void OnFinish() {}
        virtual void OnPreDraw() {}
            
        void Init(const APIPtr& api, const FunctionPtr<void()>& nextBatchFn) override;
        void Finish() override;
        void SubmitVertex(Vertex& vertex);
        void ResetBatch() override;
        void Draw() override;
    
    protected:
        virtual void SetBufferElements(VertexBuffer& VBO) const = 0;
        
    private:
        SharedPtr<IndexBuffer> CreateQuadIndexBuffer(uint32_t maxPrimitives);
        
        bool                      bInitialized    = false;
        APIPtr                    m_API           = nullptr;
        FunctionPtr<void()>     m_NextBatchFn   = nullptr;
        SharedPtr<VertexArray>    VAO             = nullptr;
        SharedPtr<VertexBuffer>   VBO             = nullptr;
        SharedPtr<IndexBuffer>    IBO             = nullptr;
        
        uint32_t                  MaxVertices     = 0;
        uint32_t                  MaxIndices      = 0;
        uint32_t                  IndexCount      = 0;
        Vertex*                   Vertices        = nullptr;
        Vertex*                   LastVertex      = 0;
        uint32_t                  CurrentIndex    = 0;
    };
    
    template <typename Vertex>
    void RenderSystem2D<Vertex>::Init(const APIPtr& api, const FunctionPtr<void()>& nextBatchFn)
    {
        m_API = api;
        m_NextBatchFn = nextBatchFn;
        
        if (bInitialized)
        {
            return;
        }
        
        bInitialized = true;
        MaxVertices = GetMaxPrimitives() * VerticesPerPrimitive;
        MaxIndices  = GetMaxPrimitives() * IndicesPerPrimitive;
        IndexCount  = 0;
        Vertices    = new Vertex[MaxVertices];
        LastVertex  = Vertices;

        const GraphicsAPI graphicsAPI = m_API->GetGraphicsAPI();
        VAO = VertexArray::Create(graphicsAPI);
        VBO = VertexBuffer::Create(graphicsAPI, MaxVertices * sizeof(Vertex));
        
        SetBufferElements(*VBO.get());

        VAO->AddVertexBuffer(VBO);
        IBO = CreateQuadIndexBuffer(GetMaxPrimitives());
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
    void RenderSystem2D<Vertex>::ResetBatch()
    {
        // reset vertex data
        LastVertex = Vertices;
        IndexCount = 0;
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::Draw()
    {
        if (const uint32_t vertexDataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(LastVertex) -
               reinterpret_cast<uint8_t*>(Vertices)))
        {
            VBO->SetData(Vertices, vertexDataSize);
            OnPreDraw();
            CommandQueue::Submit<DrawElementsCommand>(m_API, VAO, IndexCount);
        }
        
        while (!CommandQueue::IsEmpty())
        {
            CommandQueue::ExecuteNext();
        }
    }

    template <typename Vertex>
    void RenderSystem2D<Vertex>::SubmitVertex(Vertex& vertex)
    {
        *LastVertex = vertex;
        LastVertex++;
        CurrentIndex++;
        
        if (CurrentIndex == VerticesPerPrimitive)
        {
            CurrentIndex = 0; 
        }

        IndexCount += IndicesPerPrimitive;
        
        if (IndexCount + IndicesPerPrimitive >= MaxIndices)
        {
            m_NextBatchFn();
        }
    }
    
    template <typename Vertex>
    SharedPtr<IndexBuffer> RenderSystem2D<Vertex>::CreateQuadIndexBuffer(uint32_t maxPrimitives)
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
        const GraphicsAPI graphicsAPI = m_API->GetGraphicsAPI();
        auto idxBuffer = IndexBuffer::Create(graphicsAPI, indices, maxIndices);
        delete[] indices;
        return idxBuffer;
    }
}