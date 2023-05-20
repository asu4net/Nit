#pragma once
#include "ActorComponent.h"
#include "Core/Asset/AssetLink.h"
#include "Rendering/Data/Texture2D.h"

namespace Nit
{
    struct SpriteComponent : ActorComponent
    {
        SpriteComponent() = default;
        
        SpriteComponent(const AssetLink<Texture2D>& textureLink)
            : TextureLink(textureLink)
        {}

        SpriteComponent(const Vec4& color)
            : Color(color)
        {}
        
        AssetLink<Texture2D> TextureLink;
        Vec4 Color = White;
        bool bIsSubTexture = false;
        Vec2 SubTextureSize = VecOne;
        Vec2 LocationInAtlas = VecZero;
        Vec2 UVScale = VecOne;
        Vec2 Size = VecOne;
        Flip Flip = Flip::None;
        
        RTTR_ENABLE(ActorComponent)       
    };
    NIT_FORCE_LINK(SpriteComponent)
}