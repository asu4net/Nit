#include "OpenGLRenderAPI.h"
#include "NitRender/Objects/VertexArray.h"
#include <glad/glad.h>

namespace Nit
{
    void COpenGlRenderAPI::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) const
    {
        glViewport(x, y, width, height);
    }

    void COpenGlRenderAPI::SetClearColor(const CColor& clearColor) const
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }

    void COpenGlRenderAPI::Clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void COpenGlRenderAPI::SetBlendingEnabled(const bool bEnabled) const
    {
        if (bEnabled) glEnable(GL_BLEND);
        else glDisable(GL_BLEND);
    }

    void COpenGlRenderAPI::SetBlendingMode(const EBlendingMode blendingMode) const
    {
        switch (blendingMode) {
        case EBlendingMode::Solid:
            glBlendFunc(GL_ONE, GL_ZERO);
            break;
        case EBlendingMode::Alpha:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case EBlendingMode::Add:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case EBlendingMode::Multiply:
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        }
    }

    void COpenGlRenderAPI::DrawElements(const TSharedPtr<CVertexArray>& vertexArray, const uint32_t elementCount) const 
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();
    }

    void COpenGlRenderAPI::SetDepthTestEnabled(const bool bEnabled) const 
    {
        if (bEnabled) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }
}
