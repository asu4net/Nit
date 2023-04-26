#include "RendererAPI.h"
#include "Data/VertexArray.h"
#include <glad/glad.h>

namespace Nit
{
    RendererAPI* RendererAPI::Create()
    {
        return new RendererAPI();
    }

    void RendererAPI::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void RendererAPI::SetClearColor(const glm::vec4& clearColor)
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }

    void RendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererAPI::SetBlendingEnabled(const bool bEnabled)
    {
        if (bEnabled) glEnable(GL_BLEND);
        else glDisable(GL_BLEND);
    }

    void RendererAPI::SetBlendingMode(const BlendingMode blendingMode)
    {
        switch (blendingMode) {
        case BlendingMode::Solid:
            glBlendFunc(GL_ONE, GL_ZERO);
            break;
        case BlendingMode::Alpha:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case BlendingMode::Add:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case BlendingMode::Multiply:
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        }
    }

    void RendererAPI::DrawElements(const Shared<VertexArray>& vertexArray, const uint32_t elementCount)
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();
    }

    void RendererAPI::SetDepthTestEnabled(const bool bEnabled)
    {
        if (bEnabled) glEnable(GL_DEPTH_TEST);  
        else glDisable(GL_DEPTH_TEST);
    }
}
