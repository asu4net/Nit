#include "AnimationComponent.h"
#include "Core/Engine.h"
#include "Entity/Entity.h"
#include "Asset/Content.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(AnimationComponent)

    AnimationComponent::AnimationComponent(const TString& animationAssetName)
        : CurrentAnimation(Content::GetAssetByName(animationAssetName))
    {}
}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<AnimationComponent>("AnimationComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("CurrentAnimation", &AnimationComponent::CurrentAnimation)
        .property("PlayOnStart", &AnimationComponent::PlayOnStart)
        .property("CurrentIndex", &AnimationComponent::CurrentIndex)
        .property("CurrentTime", &AnimationComponent::CurrentTime)
        .property("LoopEnabled", &AnimationComponent::LoopEnabled)
        .property("IsPlaying", &AnimationComponent::IsPlaying);

    ComponentReflection::RegisterComponentType<AnimationComponent>();
}