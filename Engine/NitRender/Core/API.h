#pragma once

namespace Nit::Render
{
    class VertexArray;
    
    class API
    {
    public:
        static SharedPtr<API> Create(GraphicsAPI api);
        
        virtual ~API() {};
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;
        virtual void SetClearColor(const Color& clearColor) const = 0;
        virtual void Clear() const = 0;
        virtual void SetBlendingEnabled(bool bEnabled) const = 0;
        virtual void SetBlendingMode(const BlendingMode blendingMode) const = 0;
        virtual void DrawElements(const SharedPtr<VertexArray>& vertexArray, uint32_t elementCount) const = 0;
        virtual void SetDepthTestEnabled(bool bEnabled) const = 0;
        virtual GraphicsAPI GetGraphicsAPI() const = 0;
    };

    using APIPtr = SharedPtr<API>;
}