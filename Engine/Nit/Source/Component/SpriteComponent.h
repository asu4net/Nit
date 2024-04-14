#pragma once
#include "Asset/AssetRef.h"
#include "Asset/Sprite.h"
#include "Render/RenderEntity.h"

namespace Nit
{
    struct CSpritePrimitive;

    struct SpriteComponent
    {
        bool IsVisible = true;
        int SortingLayer = 0;

        AssetRef SpriteAssetRef;
        bool bUseSubsprite = false;
        TString SubSpriteName = "";
        CVector2 Size = CVector2::One;
        CColor TintColor = CColor::White;
        bool bFlipX = false;
        bool bFlipY = false;
        CVector2 UVScale = CVector2::One;
        RenderEntity renderEntity;

        SpriteComponent() = default;

        SpriteComponent(const AssetRef& spriteAssetRef, bool bSubsprite = false)
            : SpriteAssetRef(spriteAssetRef)
            , bUseSubsprite(bSubsprite)
        {
        }
        
        SpriteComponent(const TString& spriteAssetName, bool bSubsprite = false);

        SpriteComponent(const CColor tintColor)
            : TintColor(tintColor)
        {
        }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(SpriteComponent)
}
