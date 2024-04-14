#pragma once
#include "Asset.h"

namespace Nit
{
    struct CSubSprite 
    {
        CSubSprite()
        {}

        CVector2           Size;
        TArray<CVector3, 4> VertexPositions;
        TArray<CVector2, 4> VertexUVs;

        RTTR_ENABLE()
    };
    
    struct CSpriteInitSettings
    {
        EMagnificationFilter MagFilter  = EMagnificationFilter::Linear;
        EMinificationFilter  MinFilter  = EMinificationFilter::Linear;
        ETextureWrapMode     WrapModeU  = ETextureWrapMode::Repeat;
        ETextureWrapMode     WrapModeV  = ETextureWrapMode::Repeat;
    };

    class CSprite : public Asset
    {
    public:
        static TString DefaultFolder();
        
        CSprite();

        void Init(const CSpriteInitSettings& initSettings = {});

        bool                     Load() override;
        void                     Unload() override;

        void*                    GetData() const { return m_Data; }
        Id                       GetRendererId() const { return m_RendererTextureId; }
        CVector2                  GetSize() const { return m_Size; }
        const TArray<CVector3, 4>& GetVertexPositions() const { return m_VertexPositions; }
        const TArray<CVector2, 4>& GetVertexUVs() const { return m_VertexUVs; }

        const CSubSprite&         PushSubSprite(const TString& name, const CVector2& uvMin, const CVector2& uvMax, const CVector2& size);
        const CSubSprite&         PushSubSprite(const TString& name, const CVector2& locationInAtlas, const CVector2& size);
        void                     PushSpriteSheet(const CVector2& atlasTileSize, uint32_t numOfTiles, bool atlasIsHorizontal = true);
        void                     PopSubSprite(const TString& name);
        const CSubSprite&         GetSubSprite(const TString& name);
        size_t                   GetNumOfSubSprites() const { return m_SubSprites.size(); }
        bool                     HasSubSprites() const { return !m_SubSprites.empty(); }
        bool                     ContainsSubSprite(const TString& name);
        void                     ForEachSubSprite(Delegate<void(const TString&, const CSubSprite&)> eachDelegate);
        void                     ClearSubSprites();
        
    private:
        void*                      m_Data = nullptr;
        Id                         m_RendererTextureId = 0;
        CVector2                   m_Size;
        uint32_t                   m_Channels = 0;
        EMagnificationFilter       m_MagFilter = EMagnificationFilter::Linear;
        EMinificationFilter        m_MinFilter = EMinificationFilter::Linear;
        ETextureWrapMode           m_WrapModeU = ETextureWrapMode::Repeat;
        ETextureWrapMode           m_WrapModeV = ETextureWrapMode::Repeat;
        TArray<CVector3, 4>        m_VertexPositions;
        TArray<CVector2, 4>        m_VertexUVs;
        TMap<TString, CSubSprite>  m_SubSprites;
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    NIT_FORCE_LINK(Sprite)
}