#pragma once
#include "RenderEntity.h"
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
    enum Primitive2DType
    {
        Primitive2DType_Default,
        Primitive2DType_Sprite,
        Primitive2DType_Circle,
        Primitive2DType_Line,
        Primitive2DType_Rect
    };

    struct Primitive2D
    {
        virtual Primitive2DType GetType() const { return Primitive2DType_Default; };
        bool IsType(Primitive2DType type) const { return GetType() == type; }

        bool              bIsVisible      = false;
        Matrix4           Transform;      // Identity
        Color             TintColor       = Color::White;
        Array<Vector3, 4> VertexPositions = Render::GetQuadVertexPositions();
        Array<Vector2, 4> VertexUVs       = Render::GetQuadVertexUVs();
        Id                ShaderID        =  0;
        int               EntityID        = -1;
        int               SortingLayer    =  0;
    };

    struct SpritePrimitive : Primitive2D
    {
        virtual Primitive2DType GetType() const override { return Primitive2DType_Sprite; }
        
        Id                TextureID = 0;
        Vector2           Size = Vector2::One;
        bool              bFlipX = false;
        bool              bFlipY = false;
        Vector2           UVScale = Vector2::One;
    };

    struct CirclePrimitive : Primitive2D
    {
        virtual Primitive2DType GetType() const override { return Primitive2DType_Circle; }

        float Radius    = .5f;
        float Thickness = .05f;
        float Fade      = .01f;
    };

    struct LinePrimitive : Primitive2D
    {
        virtual Primitive2DType GetType() const override { return Primitive2DType_Line; }
        
        Vector2 Start   = Vector2::Zero;
        Vector2 End     = Vector2::Right;
        float Thickness = .05f;
        float Fade      = .01f;
    };
}

namespace Nit::Renderer
{
    void Init(GraphicsAPI api);
    Registry* GetRegistryPtr();
    Registry& GetRegistry();
    void SetSpriteShader(const SharedPtr<RendererShader>& spriteShader);
    const SharedPtr<RendererShader>& GetSpriteShader();
    void SetLastShader(const SharedPtr<RendererShader>& lastShader);
    const SharedPtr<RendererShader>& GetLastShader();
    RenderEntity CreateRenderEntity();
    
    void SetCircleShader(const SharedPtr<RendererShader> circleShader);
    void SetLineShader(const SharedPtr<RendererShader> lineShader);
    void SetProjectionViewMatrix(const Matrix4& matrix);
    Matrix4 GetProjectionViewMatrix();
    void SetErrorScreenEnabled(bool bEnabled);

    SharedPtr<RendererAPI> GetAPI();

    int CalculateTextureSlot(const SharedPtr<RendererTexture2D>& texture);
    void NextBatch();
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

    template<>
    inline CirclePrimitive* CreatePrimitive<CirclePrimitive>()
    {
        auto* primitive = new CirclePrimitive();
        PushPrimitive(primitive);
        return primitive;
    }

    template<>
    inline LinePrimitive* CreatePrimitive<LinePrimitive>()
    {
        auto* primitive = new LinePrimitive();
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