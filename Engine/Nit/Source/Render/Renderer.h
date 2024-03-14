#pragma once
#include "SpritePrimitive.h"

namespace Nit
{
    struct Texture2DSettings;
    class RendererShader;
    class RendererTexture2D;
    class RendererAPI;
    class Framebuffer;
    class AssetRef;
}

namespace Nit::Renderer
{
    void Init(GraphicsAPI api);
    void SetSpriteShader(const SharedPtr<RendererShader> spriteShader);
    void SetProjectionViewMatrix(const Matrix4& matrix);
    void SetBlendingModeEnabled(bool enabled);
    void SetBlendingMode(BlendingMode blendingMode);
    void SetDepthTestEnabled(bool enabled);
    void SetErrorScreenEnabled(bool bEnabled);
    void SetClearColor(const Color& clearColor);
    void Clear();
    void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height);
    void SubmitSpritePrimitive(const SpritePrimitive& sprite);
    
    void DrawPrimitives();

    Id CreateTexture2D(const Texture2DSettings& settings, const void* data);
    SharedPtr<RendererTexture2D> GetTexture2D(Id id);
    void DestroyTexture2D(Id id);

    Id CreateShader(const char* vertexSource, const char* fragmentSource);
    SharedPtr<RendererShader> GetShader(Id id);
    void DestroyShader(Id id);
}