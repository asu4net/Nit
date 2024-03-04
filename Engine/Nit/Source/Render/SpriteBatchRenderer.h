#pragma once
#include "RendererAPI.h"
#include "Asset/Sprite.h"

namespace Nit
{
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
    class RendererTexture2D;
    class RendererShader;

    struct SpriteVertex
    {
        Vector3  Position = Vector2::Zero;
        Color    TintColor = Color::White;
        Vector2  UV = Vector2::One;
        uint32_t TextureSlot = 0;
        uint32_t Shape = 0;
        Vector3  LocalPosition = Vector2::Zero;
        float    Thickness = .05f;
        float    Fade = .01f;
        Vector2  Bounds = Vector2::Zero;
        Color    RectColor = Color::Orange;
        int EntityID = -1;
    };

    class SpriteBatchRenderer
    {
    public:
        inline static uint32_t MaxSprites = 3000;
        inline static uint32_t MaxTextureSlots = 32;

        struct RenderData
        {
            Matrix4 ProjectionViewMat4;
            SharedPtr<RendererShader> SpriteShader;
        };

        static void Init(const SharedPtr<RendererAPI> API);

        static int CalculateTextureSlot(const SharedPtr<RendererTexture2D>& texture);
        static void Begin(RenderData& renderData);
        static void SubmitSpriteVertexData(const Array<SpriteVertex, 4>& spriteVertices);
        static void End();

    private:
        static void CreateWhiteTexture();
        static void CreateTextureSlots();
        static void CreateRenderObjects();
        static void StartBatch();
        static void Flush();
        
        inline static SharedPtr<RendererAPI> m_RendererAPI;

        inline static SharedPtr<VertexArray> m_VertexArray;
        inline static SharedPtr<VertexBuffer> m_VertexBuffer;
        inline static SharedPtr<IndexBuffer> m_IndexBuffer;
        inline static SharedPtr<RendererTexture2D> m_WhiteTexture;

        // Per batch data
        inline static uint32_t m_MaxVertices;
        inline static DynamicArray<int32_t> m_TextureSlots;

        inline static uint32_t m_SpriteCount = 0;
        inline static uint32_t m_IndexCount = 0;
        inline static uint32_t m_LastTextureSlot = 1;
        inline static SpriteVertex* m_Vertices;
        inline static SpriteVertex* m_LastVertex;
        inline static DynamicArray<SharedPtr<RendererTexture2D>> m_Textures;
    };
}