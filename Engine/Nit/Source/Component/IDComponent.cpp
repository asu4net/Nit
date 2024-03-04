#include "IDComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    RTTR_REGISTRATION
    {
        using namespace Nit;

        rttr::registration::class_<IDComponent>("IDComponent")
            .constructor<>()(rttr::policy::ctor::as_object)
            .property("ID", &IDComponent::ID);

        ComponentReflection::RegisterComponentType<IDComponent>();
    }
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(IDComponent)
}