#pragma once

namespace Nit
{
    class VertexArray;

    enum class BlendingMode
    {
        Alpha,
        Solid,
        Add,
        Multiply
    };
    
    class RendererAPI
    {
    public:
        static RendererAPI* Create();
        
        RendererAPI() = default;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void SetClearColor(const Vec4& clearColor);
        void Clear();
        void SetBlendingEnabled(bool bEnabled);
        void SetBlendingMode(const BlendingMode blendingMode);
        void DrawElements(const Shared<VertexArray>& vertexArray, uint32_t elementCount);
        void SetDepthTestEnabled(bool bEnabled);
    };
}
