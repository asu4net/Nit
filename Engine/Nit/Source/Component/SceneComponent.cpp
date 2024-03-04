#include "SceneComponent.h"
#include "Entity/Entity.h"
#include "ComponentReflection.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    
    rttr::registration::class_<SceneComponent>("SceneComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("OwnerScene", &SceneComponent::OwnerScene)
        .property("IsSerializable", &SceneComponent::IsSerializable);

    ComponentReflection::RegisterComponentType<SceneComponent>();
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(SceneComponent)
}