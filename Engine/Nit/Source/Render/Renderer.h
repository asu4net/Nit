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
    void SetClearColor(const Color& clearColor);
    void Clear();
    void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height);
    void SubmitSpritePrimitive(const SpritePrimitive& sprite);
    
    void SubmitQuad(const Color& color = Color::White, const Matrix4& transform = {}, const Vector2& size = Vector2::One, Flip flip = Flip::None, int EntityID = -1);
    void SubmitQuad2D(const Color& color = Color::White, const Vector2& position = Vector2::Zero, const float angle = 0, const Vector2& scale = Vector2::One, const Vector2& size = Vector2::One, Flip flip = Flip::None, int EntityID = -1);
    void SubmitLine2D(const Vector2& start, const Vector2& end, const Vector2& normal = Vector2::Up, const Color& color = Color::White, float thickness = 0.05f);
    void SubmitSprite(const String& spriteName, const Matrix4& transform = {}, const Color& tintColor = Color::White, const Vector2& size = Vector2::One, Flip flip = Flip::None, int EntityID = -1);
    void SubmitSprite2D(const String& spriteName, const Vector2& position = Vector2::Zero, const float angle = 0, const Vector2& scale = Vector2::One, const Color& tintColor = Color::White, const Vector2& size = Vector2::One, Flip flip = Flip::None, int EntityID = -1);
    void SubmitSprite(const AssetRef& spriteRef, const Matrix4& transform = {}, const Color& tintColor = Color::White, const Vector2& size = Vector2::One, Flip flip = Flip::None, int EntityID = -1);
    void SubmitSprite2D(const AssetRef& spriteRef, const Vector2& position = Vector2::Zero, const float angle = 0, const Vector2& scale = Vector2::One, const Color& tintColor = Color::White, const Vector2& size = Vector2::One, Flip flip = Flip::None, int EntityID = -1);
    void DrawPrimitives();

    Id CreateTexture2D(const Texture2DSettings& settings, const void* data);
    SharedPtr<RendererTexture2D> GetTexture2D(Id id);
    void DestroyTexture2D(Id id);

    Id CreateShader(const char* vertexSource, const char* fragmentSource);
    SharedPtr<RendererShader> GetShader(Id id);
    void DestroyShader(Id id);
}