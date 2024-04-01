#pragma once
#include "RenderEntity.h"
#include "ShaderDataType.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "RendererShader.h"
#include "RendererTexture2D.h"

#define NIT_RENDER_SYSTEM(_TYPE) \
       public:  \
       const String&    GetPrimitiveID()   const override { return #_TYPE; }; \
       private: 

namespace Nit
{
    struct BufferElement;
    class  VertexArray;
    class  VertexBuffer;
    class  IndexBuffer;
    class  RendererShader;
    class  RendererAPI;
    
    using  BufferElementList = std::initializer_list<BufferElement>;
    using  ShaderPtr = SharedPtr<RendererShader>;
    
    class IRenderSystem2D
    {
    public:
        virtual const String&    GetPrimitiveID()    const = 0;
        virtual const ShaderPtr& GetDefaultShader()  const = 0;

        virtual void Init()          = 0;
        virtual void Finish()        = 0;
        virtual void BatchVertices() = 0;
        virtual void Draw()          = 0;
    };
    
    template<typename Vertex>
    class RenderSystem2D : public IRenderSystem2D
    {
    public:
        static constexpr uint32_t  MaxPrimitives        = 3000;
        static constexpr uint32_t  VerticesPerPrimitive = 4;
        static constexpr uint32_t  IndicesPerPrimitive  = 6;

        static SharedPtr<IndexBuffer> CreateQuadIndexBuffer(uint32_t maxPrimitives)
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
        
        RenderSystem2D() = default;
        virtual ~RenderSystem2D() = default;
        
        virtual       uint32_t   GetMaxPrimitives()  const  { return MaxPrimitives; }

        virtual void OnInit() {}
        virtual void OnFinish() {}
        virtual void OnPreDraw() {}
        
        void Init()          override
        {
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

            const GraphicsAPI API = Renderer::GetAPI()->GetGraphicsAPI();
            VAO = VertexArray::Create(API);
            VBO = VertexBuffer::Create(API, MaxVertices * sizeof(Vertex));

            SetBufferElements(*VBO.get());

            VAO->AddVertexBuffer(VBO);
            IBO = CreateQuadIndexBuffer(GetMaxPrimitives());
            VAO->SetIndexBuffer(IBO);
            OnInit();
        }
        void Finish()        override
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
        void BatchVertices() override
        {
            // reset vertex data
            LastVertex = Vertices;
            IndexCount = 0;

            // submit vertex data
            SubmitVertices();
        }
        void Draw()          override
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
    
    protected:
        virtual void SetBufferElements(VertexBuffer& VBO) const = 0;
        virtual void SubmitVertices()                           = 0;
        virtual void SubmitRenderEntity(RenderEntity entity)    = 0;
        
        void SubmitVertex(Vertex& vertex)
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
                Renderer::NextBatch();
            }
        }
        
        void SubmitShader(Id shaderID)
        {
            auto defaultShader = GetDefaultShader();
            auto lastShadder = Renderer::GetLastShader();

            if (shaderID != 0)
            {
                auto shader = Renderer::GetShader(shaderID);
            
                if (shader && shader != lastShadder)
                {
                    Renderer::NextBatch();
                    shader->Bind();
                    Renderer::SetLastShader(shader);
                }
            }
            else if (defaultShader && defaultShader != lastShadder)
            {
                Renderer::NextBatch();
                defaultShader->Bind();
                Renderer::SetLastShader(defaultShader);
            }

            if (!Renderer::GetLastShader())
            {
                NIT_CHECK(false, "Missing shader!");
                return;
            }
        }
        
    private:
        bool                      bInitialized    = false;
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
}