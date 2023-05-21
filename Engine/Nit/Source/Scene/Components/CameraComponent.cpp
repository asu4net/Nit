#include "CameraComponent.h"

namespace Nit { NIT_FORCE_LINK_IMPL(CameraComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::class_<CameraComponent>("CameraComponent")
        .constructor<>()
        .property("CameraData", &CameraComponent::CameraData)
        .property("Priority", &CameraComponent::Priority);
}