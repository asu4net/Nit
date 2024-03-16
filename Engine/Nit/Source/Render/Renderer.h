#pragma once
#include "RenderUtils.h"

namespace Nit
{
    struct Texture2DSettings;
    class RendererShader;
    class RendererTexture2D;
    class RendererAPI;
    class Framebuffer;
    class AssetRef;
}

namespace Nit
{
    enum Primitive2D_Type
    {
        Primitive2DType_Default,
        Primitive2DType_Sprite,
        Primitive2DType_Circle,
        Primitive2DType_Line,
        Primitive2DType_Rect
    };

    struct Primitive2D
    {
        virtual Primitive2D_Type GetType() const { return Primitive2DType_Default; };
        bool IsType(Primitive2D_Type type) const { return GetType() == type; }

        bool              bIsVisible      = false;
        Matrix4           Transform;      // Identity
        Color             TintColor       = Color::White;
        Array<Vector3, 4> VertexPositions = RenderUtils::GetQuadVertexPositions();
        Array<Vector2, 4> VertexUVs       = RenderUtils::GetQuadVertexUVs();
        Id                ShaderID        =  0;
        int               EntityID        = -1;
        int               SortingLayer    = 0;
    };

    struct SpritePrimitive : Primitive2D
    {
        virtual Primitive2D_Type GetType() const override { return Primitive2DType_Sprite; }
        
        Id                TextureID = 0;
        Vector2           Size = Vector2::One;
        bool              bFlipX = false;
        bool              bFlipY = false;
        Vector2           UVScale = Vector2::One;
    };
}

namespace Nit::Renderer
{
    void Init(GraphicsAPI api);
    void SetSpriteShader(const SharedPtr<RendererShader> spriteShader);
    void SetProjectionViewMatrix(const Matrix4& matrix);
    void SetErrorScreenEnabled(bool bEnabled);

    SharedPtr<RendererAPI> GetAPI();

    void DrawPrimitives();

    void PushPrimitive(Primitive2D* primitive);
    void PopPrimitive(Primitive2D* primitive);

    template<typename T>
    inline T* CreatePrimitive()
    {
        return nullptr;
    }

    template<>
    inline SpritePrimitive* CreatePrimitive<SpritePrimitive>()
    {
        auto* primitive = new SpritePrimitive();
        PushPrimitive(primitive);
        return primitive;
    }

    void DestroyPrimitive(Primitive2D* primitive);

    Id CreateTexture2D(const Texture2DSettings& settings, const void* data);
    SharedPtr<RendererTexture2D> GetTexture2D(Id id);
    void DestroyTexture2D(Id id);

    Id CreateShader(const char* vertexSource, const char* fragmentSource);
    SharedPtr<RendererShader> GetShader(Id id);
    void DestroyShader(Id id);
}