#pragma once
#include "RenderEntity.h"
#include "ShaderDataType.h"
#include "VertexBuffer.h"

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
    
    template<typename Vertex>
    class RenderSystem2D
    {
    public:
        RenderSystem2D() = default;
        virtual ~RenderSystem2D() = default;
        
        virtual const String&    GetPrimitiveID()    const = 0;
        virtual const ShaderPtr& GetDefaultShader()  const = 0;
        virtual       uint32_t   GetMaxPrimitives()  const;

        virtual void OnInit();
        virtual void OnFinish();
        virtual void OnPreDraw();
        
        void Init();
        void Finish();
        void BatchVertices();
        void Draw();
    
    protected:
        virtual void SetBufferElements(VertexBuffer& VBO) const = 0;
        virtual void SubmitVertices()                           = 0;
        
        void SubmitVertex(Vertex& vertex);
        
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
