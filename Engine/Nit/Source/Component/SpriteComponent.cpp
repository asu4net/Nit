#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Entity/Entity.h"
#include "Core/Engine.h"
#include "Asset/Content.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(SpriteComponent);

    SpriteComponent::SpriteComponent(const String& spriteAssetName, bool bSubsprite)
        : SpriteAssetRef(Content::GetAssetByName(spriteAssetName))
        , bUseSubsprite(bSubsprite)
    {
    }
}

RTTR_REGISTRATION
{
    using namespace Nit;
    
    rttr::registration::class_<SpriteComponent>("SpriteComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("IsVisible", &SpriteComponent::IsVisible)
        .property("SortingLayer", &SpriteComponent::SortingLayer)
        .property("SpriteAssetRef", &SpriteComponent::SpriteAssetRef)
        .property("UseSubsprite", &SpriteComponent::bUseSubsprite)
        .property("SubSpriteName", &SpriteComponent::SubSpriteName)
        .property("Size", &SpriteComponent::Size)
        .property("TintColor", &SpriteComponent::TintColor)
        .property("FlipX", &SpriteComponent::bFlipX)
        .property("FlipY", &SpriteComponent::bFlipY)
        .property("UVScale", &SpriteComponent::UVScale);

    ComponentReflection::RegisterComponentType<SpriteComponent>();
}