#pragma once

namespace Nit
{
    class CVertexArray;
    
    class CRenderAPI
    {
    public:
        static TSharedPtr<CRenderAPI> Create(EGraphicsAPI api);
        
        virtual ~CRenderAPI() = default;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;
        virtual void SetClearColor(const CColor& clearColor) const = 0;
        virtual void Clear() const = 0;
        virtual void SetBlendingEnabled(bool bEnabled) const = 0;
        virtual void SetBlendingMode(const EBlendingMode blendingMode) const = 0;
        virtual void DrawElements(const TSharedPtr<CVertexArray>& vertexArray, uint32_t elementCount) const = 0;
        virtual void SetDepthTestEnabled(bool bEnabled) const = 0;
        virtual EGraphicsAPI GetGraphicsAPI() const = 0;
    };

    using TRenderAPIPtr = TSharedPtr<CRenderAPI>;
}