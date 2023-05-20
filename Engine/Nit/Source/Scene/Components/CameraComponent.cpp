#include "CameraComponent.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::class_<CameraComponent>("CameraComponent")
        .constructor<>()
        .property("CameraData", &CameraComponent::CameraData)
        .property("Priority", &CameraComponent::Priority);
}