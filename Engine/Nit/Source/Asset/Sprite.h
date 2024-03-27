#pragma once
#include "AssetRef.h"
#include "Asset.h"

namespace Nit
{
    struct SubSprite 
    {
        SubSprite()
        {}

        Vector2           Size;
        Array<Vector3, 4> VertexPositions;
        Array<Vector2, 4> VertexUVs;

        RTTR_ENABLE()
    };
    
    struct SpriteInitSettings
    {
        MagnificationFilter magFilter  = MagnificationFilter::Linear;
        MinificationFilter  minFilter  = MinificationFilter::Linear;
        TextureWrapMode     wrapModeU  = TextureWrapMode::Repeat;
        TextureWrapMode     wrapModeV  = TextureWrapMode::Repeat;
    };

    class Sprite : public Asset
    {
    public:
        static String DefaultFolder();
        
        Sprite();

        void Init(const SpriteInitSettings& initSettings = {});

        bool                     Load() override;
        void                     Unload() override;

        void*                    GetData() const { return m_Data; }
        Id                       GetRendererId() const { return m_RendererTextureId; }
        Vector2                  GetSize() const { return m_Size; }
        const Array<Vector3, 4>& GetVertexPositions() const { return m_VertexPositions; }
        const Array<Vector2, 4>& GetVertexUVs() const { return m_VertexUVs; }

        const SubSprite&         PushSubSprite(const String& name, const Vector2& uvMin, const Vector2& uvMax, const Vector2& size);
        const SubSprite&         PushSubSprite(const String& name, const Vector2& locationInAtlas, const Vector2& size);
        void                     PushSpriteSheet(const Vector2& atlasTileSize, uint32_t numOfTiles, bool atlasIsHorizontal = true);
        void                     PopSubSprite(const String& name);
        const SubSprite&         GetSubSprite(const String& name);
        size_t                   GetNumOfSubSprites() const { return m_SubSprites.size(); }
        bool                     HasSubSprites() const { return !m_SubSprites.empty(); }
        bool                     ContainsSubSprite(const String& name);
        void                     ForEachSubSprite(Delegate<void(const String&, const SubSprite&)> eachDelegate);
        void                     ClearSubSprites();
        
    private:
        void*                   m_Data = nullptr;
        Id                      m_RendererTextureId = 0;
        Vector2                 m_Size;
        uint32_t                m_Channels = 0;
        MagnificationFilter     m_MagFilter = MagnificationFilter::Linear;
        MinificationFilter      m_MinFilter = MinificationFilter::Linear;
        TextureWrapMode         m_WrapModeU = TextureWrapMode::Repeat;
        TextureWrapMode         m_WrapModeV = TextureWrapMode::Repeat;
        Array<Vector3, 4>       m_VertexPositions;
        Array<Vector2, 4>       m_VertexUVs;
        Map<String, SubSprite>  m_SubSprites;
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    NIT_FORCE_LINK(Sprite)
}