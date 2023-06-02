#include "SpriteComponent.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace Nit { NIT_FORCE_LINK_IMPL(SpriteComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<SpriteComponent>("SpriteComponent")
        .constructor<>()
        .property("TextureLink", &SpriteComponent::TextureLink)
        .property("Color", &SpriteComponent::Color)
        .property("IsSubTexture", &SpriteComponent::bIsSubTexture)
        .property("SubTextureSize", &SpriteComponent::SubTextureSize)
        .property("LocationInAtlas", &SpriteComponent::LocationInAtlas)
        .property("UVScale", &SpriteComponent::UVScale)
        .property("Size", &SpriteComponent::Size)
        .property("Flip", &SpriteComponent::Flip)
        .property("OrderInLayer", &SpriteComponent::OrderInLayer);

    NIT_REGISTRY_COMPONENT(SpriteComponent)
}