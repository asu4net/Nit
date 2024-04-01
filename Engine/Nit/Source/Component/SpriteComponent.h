#pragma once
#include "Asset/AssetRef.h"
#include "Asset/Sprite.h"
#include "Render/RenderEntity.h"

namespace Nit
{
    struct SpritePrimitive;

    struct SpriteComponent
    {
        bool IsVisible = true;
        int SortingLayer = 0;

        AssetRef SpriteAssetRef;
        bool bUseSubsprite = false;
        String SubSpriteName = "";
        Vector2 Size = Vector2::One;
        Color TintColor = Color::White;
        bool bFlipX = false;
        bool bFlipY = false;
        Vector2 UVScale = Vector2::One;
        RenderEntity renderEntity;

        SpriteComponent() = default;

        SpriteComponent(const AssetRef& spriteAssetRef, bool bSubsprite = false)
            : SpriteAssetRef(spriteAssetRef)
            , bUseSubsprite(bSubsprite)
        {
        }
        
        SpriteComponent(const String& spriteAssetName, bool bSubsprite = false);

        SpriteComponent(const Color tintColor)
            : TintColor(tintColor)
        {
        }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(SpriteComponent)
}
