#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Entity/Entity.h"
#include "Core/Engine.h"
#include "Asset/Content.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(SpriteComponent);

    SpriteComponent::SpriteComponent(const String& spriteAssetName, SpriteSource source)
        : SpriteAssetRef(Content::GetAssetByName(spriteAssetName))
        , Source(source)
    {
    }
}

RTTR_REGISTRATION
{
    using namespace Nit;
    
    rttr::registration::class_<SpriteComponent>("SpriteComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("IsVisible", &SpriteComponent::IsVisible)
        .property("OrderInLayer", &SpriteComponent::OrderInLayer)
        .property("SpriteAssetRef", &SpriteComponent::SpriteAssetRef)
        .property("Source", &SpriteComponent::Source)
        .property("SubSpriteName", &SpriteComponent::SubSpriteName)
        .property("Size", &SpriteComponent::Size)
        .property("TintColor", &SpriteComponent::TintColor)
        .property("FlipMode", &SpriteComponent::FlipMode)
        .property("UVScale", &SpriteComponent::UVScale);

    ComponentReflection::RegisterComponentType<SpriteComponent>();
}