#pragma once
#include "NitRender/Core/RenderAPI.h"

namespace Nit
{
    class CVertexArray;

    class COpenGlRenderAPI : public CRenderAPI
    {
    public:
        COpenGlRenderAPI() = default;
        ~COpenGlRenderAPI() override = default;

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;
        void SetClearColor(const CColor& clearColor) const override;
        void Clear() const override;
        void SetBlendingEnabled(bool bEnabled) const override;
        void SetBlendingMode(const EBlendingMode blendingMode) const override;
        void DrawElements(const TSharedPtr<CVertexArray>& vertexArray, uint32_t elementCount) const override;
        void SetDepthTestEnabled(bool bEnabled) const override;
        EGraphicsAPI GetGraphicsAPI() const override { return EGraphicsAPI::OpenGL; }
    };
}
