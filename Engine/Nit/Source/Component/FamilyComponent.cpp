#include "FamilyComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(FamilyComponent);
}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<FamilyComponent>("FamilyComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("EntityRef", &FamilyComponent::ParentRef);

    ComponentReflection::RegisterComponentType<FamilyComponent>();
}