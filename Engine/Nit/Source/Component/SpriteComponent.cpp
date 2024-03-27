#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Entity/Entity.h"
#include "Core/Engine.h"
#include "Asset/Content.h"
#include "ComponentReflection.h"

/*
Graphics2DComponent
IsVisible
SortingLayer
TintColor
Primitive*

SpriteComponent
next stuff

CircleComponent
radius
thickness
smooth

Physics2DComponent

mejorar el sistema de assets. 
. unificar assets de render con los otros

Tener un render system que renderice todas las entidades que llevan un componente relacionado.
almacenar la información de las primitivas en los propios componentes. Que tenga funciones de
draw y demás que pueda usar el usuario desde fuera. Lo único que si lo hace tendrá que funcionar
en inmediate mode. (usando begin - end) por su cuenta.

*/

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