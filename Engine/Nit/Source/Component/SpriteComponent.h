#pragma once
#include "Asset/AssetRef.h"
#include "Asset/Sprite.h"

namespace Nit
{
    struct SpriteComponent
    {
        bool IsVisible = true;
        int OrderInLayer = 0; //TODO: Rename to sorting order

        AssetRef SpriteAssetRef;
        SpriteSource Source = SpriteSource::Default;    
        String SubSpriteName = "";
        Vector2 Size = Vector2::One;
        Color TintColor = Color::White;
        Flip FlipMode = Flip::None;
        Vector2 UVScale = Vector2::One;

        SpriteComponent() = default;

        SpriteComponent(const AssetRef& spriteAssetRef, SpriteSource source = SpriteSource::Default)
            : SpriteAssetRef(spriteAssetRef)
            , Source(source)
        {
        }

        SpriteComponent(const String& spriteAssetName, SpriteSource source = SpriteSource::Default);

        SpriteComponent(const Color tintColor)
            : TintColor(tintColor)
        {
        }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(SpriteComponent)
}