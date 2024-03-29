#include "FamilyComponent.h"
#include "ComponentReflection.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<FamilyComponent>("FamilyComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("ParentRef", &FamilyComponent::ParentRef);

    ComponentReflection::RegisterComponentType<FamilyComponent>();
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(FamilyComponent);
}