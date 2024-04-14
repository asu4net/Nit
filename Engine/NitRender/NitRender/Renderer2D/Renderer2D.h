#pragma once
#include "NitRender/Renderer2D/Render2DState.h"
#include "NitRender/Renderer2D/Primitive2D.h"
#include "NitRender/Core/RenderCommand.h"
#include "NitRender/Core/RenderAPI.h"
#include "NitRender/Objects/VertexArray.h"
#include "NitRender/Objects/VertexBuffer.h"
#include "NitRender/Objects/IndexBuffer.h"
#include "NitRender/Objects/Shader.h"
#include "NitRender/Objects/Texture2D.h"

#define NIT_RENDER_SYSTEM(_TYPE) \
       public:  \
       const String&    GetPrimitiveID()   const override { return #_TYPE; }; \
       private: 

namespace Nit
{
    class IRenderer2D
    {
    public:
        virtual TString    GetPrimitiveID()    const = 0;
        
        virtual void Init(const TRenderAPIPtr& api, const TRender2DStatePtr& render2DState, const TShaderPtr& defaultShader) = 0;
        virtual void Finish()                                         = 0;
        virtual void SubmitPrimitive(const CPrimitive2D& primitive2D) = 0;
        virtual void ResetBatch()                                     = 0;
        virtual void SubmitBatchToGPU()                               = 0;
    };
    
    template<typename Vertex>
    class CRenderer2D : public IRenderer2D
    {
    public:
        static constexpr uint32_t  MaxPrimitives        = 3000;
        static constexpr uint32_t  VerticesPerPrimitive = 4;
        static constexpr uint32_t  IndicesPerPrimitive  = 6;
        
        CRenderer2D() = default;
        
        virtual ~CRenderer2D() = default;
        
        void SetDefaultShader(const TShaderPtr& defaultShader) { m_DefaultShader = defaultShader; }
        TShaderPtr GetDefaultShader() const { return m_DefaultShader; }
        
        virtual uint32_t GetMaxPrimitives() const { return MaxPrimitives; }
        const TRender2DStatePtr& GetRender2DState() const { return m_Render2DState; }
        
        void Init(const TRenderAPIPtr& api, const TRender2DStatePtr& render2DState, const TShaderPtr& defaultShader) override;
        void Finish() override;
        void SubmitVertex(Vertex& vertex);
        void ResetBatch() override;
        void SubmitBatchToGPU() override;
    
    protected:
        virtual void SetBufferElements(CVertexBuffer& VBO) const = 0;
        
    private:
        TSharedPtr<CIndexBuffer> CreateQuadIndexBuffer(uint32_t maxPrimitives);
        
        bool                      m_BInitialized    = false;
        TRenderAPIPtr             m_API             = nullptr;
        TSharedPtr<CVertexArray>  m_VAO             = nullptr;
        TSharedPtr<CVertexBuffer> m_VBO             = nullptr;
        TSharedPtr<CIndexBuffer>  m_IBO             = nullptr;
        TRender2DStatePtr         m_Render2DState   = nullptr;
        TShaderPtr                m_DefaultShader   = nullptr;
        
        uint32_t                  m_MaxVertices     = 0;
        uint32_t                  m_MaxIndices      = 0;
        uint32_t                  m_IndexCount      = 0;
        Vertex*                   m_Vertices        = nullptr;
        Vertex*                   m_LastVertex      = nullptr;
        uint32_t                  m_CurrentIndex    = 0;
    };
    
    template <typename Vertex>
    void CRenderer2D<Vertex>::Init(const TRenderAPIPtr& api, const TRender2DStatePtr& render2DState, const TShaderPtr& defaultShader)
    {
        m_API = api;
        m_Render2DState = render2DState;
        m_DefaultShader = defaultShader;
        
        if (m_BInitialized)
        {
            return;
        }
        
        m_BInitialized = true;
        m_MaxVertices = GetMaxPrimitives() * VerticesPerPrimitive;
        m_MaxIndices  = GetMaxPrimitives() * IndicesPerPrimitive;
        m_IndexCount  = 0;
        m_Vertices    = new Vertex[m_MaxVertices];
        m_LastVertex  = m_Vertices;

        const EGraphicsAPI graphicsAPI = m_API->GetGraphicsAPI();
        m_VAO = CVertexArray::Create(graphicsAPI);
        m_VBO = CVertexBuffer::Create(graphicsAPI, m_MaxVertices * sizeof(Vertex));
        
        SetBufferElements(*m_VBO.get());

        m_VAO->AddVertexBuffer(m_VBO);
        m_IBO = CreateQuadIndexBuffer(GetMaxPrimitives());
        m_VAO->SetIndexBuffer(m_IBO);
    }

    template <typename Vertex>
    void CRenderer2D<Vertex>::Finish()
    {
        if (!m_BInitialized)
        {
            return;
        }

        m_BInitialized = false;
        m_MaxVertices = 0;
        m_MaxIndices  = 0;
        m_IndexCount  = 0;
        delete[] m_Vertices;
        m_Vertices    = nullptr;
        m_LastVertex  = nullptr;
    }

    template <typename Vertex>
    void CRenderer2D<Vertex>::ResetBatch()
    {
        // reset vertex data
        m_LastVertex = m_Vertices;
        m_IndexCount = 0;
    }

    template <typename Vertex>
    void CRenderer2D<Vertex>::SubmitBatchToGPU()
    {
        if (const uint32_t vertexDataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(m_LastVertex) -
               reinterpret_cast<uint8_t*>(m_Vertices)))
        {
            m_VBO->SetData(m_Vertices, vertexDataSize);
            CRenderCommandQueue::Submit<DrawElementsCommand>(m_API, m_VAO, m_IndexCount);
        }
        
        while (!CRenderCommandQueue::IsEmpty())
        {
            CRenderCommandQueue::ExecuteNext();
        }
    }

    template <typename Vertex>
    void CRenderer2D<Vertex>::SubmitVertex(Vertex& vertex)
    {
        *m_LastVertex = vertex;
        ++m_LastVertex;
        m_CurrentIndex++;
        
        if (m_CurrentIndex == VerticesPerPrimitive)
        {
            m_CurrentIndex = 0; 
        }

        m_IndexCount += IndicesPerPrimitive;
        
        if (m_IndexCount + IndicesPerPrimitive >= m_MaxIndices)
        {
            m_Render2DState->NextBatch();
        }
    }
    
    template <typename Vertex>
    TSharedPtr<CIndexBuffer> CRenderer2D<Vertex>::CreateQuadIndexBuffer(uint32_t maxPrimitives)
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
        const EGraphicsAPI graphicsAPI = m_API->GetGraphicsAPI();
        auto idxBuffer = CIndexBuffer::Create(graphicsAPI, indices, maxIndices);
        delete[] indices;
        return idxBuffer;
    }
    
    class CSpriteRenderer : public CRenderer2D<CSpriteVertex>
    {
        public: TString GetPrimitiveID() const override { return "SpriteRenderSystem"; }; private:
        
    public:
        void SubmitPrimitive(const CPrimitive2D& primitive2D) override;

    protected:
        void SetBufferElements(CVertexBuffer& VBO) const override;
    };
}