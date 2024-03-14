#include "Renderer.h"
#include "RendererShader.h"
#include "RendererTexture2D.h"
#include "Asset/RawShaderStrings.h"
#include "SpriteBatchRenderer.h"
#include "SpritePrimitive.h"
#include "RenderCommand.h"
#include "Core/Engine.h"
#include "Asset/Content.h"

namespace Nit::Renderer
{
    Matrix4 ProjectionViewMatrix;
    SharedPtr<RendererAPI> API;
    Map<Id, SharedPtr<RendererTexture2D>> Textures;
    Map<Id, SharedPtr<RendererShader>> Shaders;

    SharedPtr<RendererShader> SpriteShader;
    SharedPtr<RendererShader> ErrorShader;

    DynamicArray<SpritePrimitive> SpritePrimitivesDrawList;
    bool bErrorScreenEnabled = false;

    void Init(GraphicsAPI api)
    {
        API = RendererAPI::Create(api);
        ErrorShader = RendererShader::Create(api);
        SpriteShader = RendererShader::Create(api);

        SpriteBatchRenderer::Init(API);

        NIT_LOG_TRACE("Renderer initialized!\n");

        // Default settings
        SetBlendingModeEnabled(true);
        SetBlendingMode(BlendingMode::Alpha);
        SetClearColor(Color::DarkGrey);

        ErrorShader->Compile(g_ErrorVertexSource, g_ErrorFragmentSource);
        SpriteShader->Compile(g_SpriteVertexShaderSource, g_SpriteFragmentShaderSource);
    }

    void SetBlendingModeEnabled(bool enabled)
    {
        RenderCommandQueue::Submit<SetBlendingEnabledCommand>(API, enabled);
    }

    void SetBlendingMode(BlendingMode blendingMode)
    {
        RenderCommandQueue::Submit<SetBlendingModeCommand>(API, blendingMode);
    }

    void SetDepthTestEnabled(bool enabled)
    {
        RenderCommandQueue::Submit<SetDepthTestEnabledCommand>(API, enabled);
    }

    void SetErrorScreenEnabled(bool bEnabled)
    {
        bErrorScreenEnabled = bEnabled;
    }

    void SetClearColor(const Color& clearColor)
    {
        RenderCommandQueue::Submit<SetClearColorCommand>(API, clearColor);
    }

    void Clear()
    {
        RenderCommandQueue::Submit<ClearCommand>(API);
    }

    void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
    {
        RenderCommandQueue::Submit<SetViewPortCommand>(API, x, y, width, height);
    }

    void SubmitSpritePrimitive(const SpritePrimitive& sprite)
    {
        SpritePrimitivesDrawList.emplace_back(sprite);
    }

    void SubmitQuad(const Color& color, const Matrix4& transform, const Vector2& size, Flip flip, int EntityID)
    {
        SpritePrimitive p;
        p.TintColor = color;
        p.TransformMatrix = transform;
        p.UVScale = size;
        p.EntityID = EntityID;
        p.FlipMode = flip;
        p.GenerateVertexData(SpriteShape::Defaut);
        SubmitSpritePrimitive(p);
    }

    void SubmitQuad2D(const Color& color, const Vector2& position, const float angle, const Vector2& scale, const Vector2& size, Flip flip, int EntityID)
    {
        SpritePrimitive p;
        p.TintColor = color;
        p.TransformMatrix = Matrix4::CreateTransform(position, {0, 0, angle}, scale);
        p.UVScale = size;
        p.EntityID = EntityID;
        p.FlipMode = flip;
        p.GenerateVertexData(SpriteShape::Defaut);
        SubmitSpritePrimitive(p);
    }

    void SubmitLine2D(const Vector2& start, const Vector2& end, const Vector2& normal, const Color& color, float thickness)
    {
        SpritePrimitive p;
        p.StartPosition = start;
        p.EndPosition = end;
        p.TintColor = color;
        p.NormalVector = normal;
        p.Thickness = thickness;
        p.GenerateVertexData(SpriteShape::Line);
        SubmitSpritePrimitive(p);
    }

    void SubmitSprite(const String& spriteName, const Matrix4& transform, const Color& tintColor, const Vector2& size, Flip flip, int EntityID)
    {
        SpritePrimitive p;
        p.SpriteRef = Content::GetAssetByName(spriteName).GetWeakAs<Sprite>().lock();
        p.TintColor = tintColor;
        p.TransformMatrix = transform;
        p.UVScale = size;
        p.EntityID = EntityID;
        p.FlipMode = flip;
        p.GenerateVertexData(SpriteShape::Defaut);
        SubmitSpritePrimitive(p);
    }

    void SubmitSprite2D(const String& spriteName, const Vector2& position, const float angle, const Vector2& scale, const Color& tintColor, const Vector2& size, Flip flip, int EntityID)
    {
        SpritePrimitive p;
        p.SpriteRef = Content::GetAssetByName(spriteName).GetWeakAs<Sprite>().lock();
        p.TintColor = tintColor;
        p.TransformMatrix = Matrix4::CreateTransform(position, { 0, 0, angle }, scale);
        p.UVScale = size;
        p.EntityID = EntityID;
        p.FlipMode = flip;
        p.GenerateVertexData(SpriteShape::Defaut);
        SubmitSpritePrimitive(p);
    }

    void SubmitSprite(const AssetRef& spriteRef, const Matrix4& transform, const Color& tintColor, const Vector2& size, Flip flip, int EntityID)
    {
        SpritePrimitive p;
        p.SpriteRef = spriteRef.GetWeakAs<Sprite>().lock();
        p.TintColor = tintColor;
        p.TransformMatrix = transform;
        p.UVScale = size;
        p.EntityID = EntityID;
        p.FlipMode = flip;
        p.GenerateVertexData(SpriteShape::Defaut);
        SubmitSpritePrimitive(p);
    }

    void SubmitSprite2D(const AssetRef& spriteRef, const Vector2& position, const float angle, const Vector2& scale, const Color& tintColor, const Vector2& size, Flip flip, int EntityID)
    {
        SpritePrimitive p;
        p.SpriteRef = spriteRef.GetWeakAs<Sprite>().lock();
        p.TintColor = tintColor;
        p.TransformMatrix = Matrix4::CreateTransform(position, { 0, 0, angle }, scale);
        p.UVScale = size;
        p.EntityID = EntityID;
        p.FlipMode = flip;
        p.GenerateVertexData(SpriteShape::Defaut);
        SubmitSpritePrimitive(p);
    }

    void DrawPrimitives()
    {
        SpriteBatchRenderer::RenderData spriteRenderData{
            ProjectionViewMatrix,
            SpriteShader,
        };

        SetClearColor(bErrorScreenEnabled ? Color::Black : Color::DarkGrey);
        Clear();

        if (bErrorScreenEnabled)
        {
            while (!RenderCommandQueue::IsEmpty())
            {
                RenderCommandQueue::ExecuteNext();
            }
            return;
        }

        SpriteBatchRenderer::Begin(spriteRenderData);

        for (const SpritePrimitive& sprite : SpritePrimitivesDrawList)
        {
            SharedPtr<RendererTexture2D> texture = nullptr;
            
            if (sprite.SpriteRef)
            {
                const Id textureId = sprite.SpriteRef->GetRendererId();
                texture = GetTexture2D(textureId);
            }

            Array<SpriteVertex, 4> spriteVertices;

            for (uint32_t i = 0; i < 4; i++)
            {
                spriteVertices[i].Position = sprite.GetVertexPositions()[i];
                spriteVertices[i].LocalPosition = sprite.GetLocalVertexPositions()[i];
                spriteVertices[i].TintColor = sprite.GetVertexColors()[i];
                spriteVertices[i].UV = sprite.GetVertexUV()[i];
                spriteVertices[i].TextureSlot = SpriteBatchRenderer::CalculateTextureSlot(texture);
                spriteVertices[i].Shape = (uint32_t) sprite.GetFragmentShape();
                spriteVertices[i].Thickness = sprite.Thickness;
                spriteVertices[i].Fade = sprite.Fade;
                spriteVertices[i].Bounds = sprite.Bounds;
                spriteVertices[i].RectColor = sprite.RectColor;
                spriteVertices[i].EntityID = sprite.EntityID;
            }

            SpriteBatchRenderer::SubmitSpriteVertexData(spriteVertices);
        }

        SpritePrimitivesDrawList.clear();
        SpriteBatchRenderer::End();
    }

    Id CreateTexture2D(const Texture2DSettings& settings, const void* data)
    {
        Id id;
        SharedPtr<RendererTexture2D> texture = RendererTexture2D::Create(API->GetGraphicsAPI());
        texture->UploadToGPU(settings, data);
        Textures[id] = texture;
        return id;
    }

    SharedPtr<RendererTexture2D> GetTexture2D(Id id)
    {
        NIT_CHECK(Textures.count(id), "Invalid id!");
        return Textures[id];
    }

    void DestroyTexture2D(Id id)
    {
        NIT_CHECK(Textures.count(id), "Invalid id!");
        SharedPtr<RendererTexture2D> texture = Textures[id];
        Textures.erase(id);
    }
    
    Id CreateShader(const char* vertexSource, const char* fragmentSource)
    {
        Id id;
        SharedPtr<RendererShader> shader = RendererShader::Create(API->GetGraphicsAPI());
        shader->Compile(vertexSource, fragmentSource);
        Shaders[id] = shader;
        return id;
    }

    SharedPtr<RendererShader> GetShader(Id id)
    {
        NIT_CHECK(Shaders.count(id), "Invalid id!");
        return Shaders[id];
    }

    void DestroyShader(Id id)
    {
        NIT_CHECK(Shaders.count(id), "Invalid id!");
        SharedPtr<RendererShader> texture = Shaders[id];
        Shaders.erase(id);
    }

    void SetSpriteShader(const SharedPtr<RendererShader> spriteShader)
    {
        SpriteShader = spriteShader;
    }

    void SetProjectionViewMatrix(const Matrix4& matrix)
    {
        ProjectionViewMatrix = matrix;
    }
}