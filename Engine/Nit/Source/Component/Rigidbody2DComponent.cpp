#include "Rigidbody2DComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(Rigidbody2DComponent);
}

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::enumeration<EBodyType>("EBodyType")
    (
        value("Static",    EBodyType::Static),
        value("Kinematic", EBodyType::Kinematic),
        value("Dynamic",   EBodyType::Dynamic)
    );
    
    registration::class_<Rigidbody2DComponent>("Rigidbody2DComponent")
        .constructor<>()(policy::ctor::as_object)
        .property("BodyType",     &Rigidbody2DComponent::BodyType)
        .property("Mass",         &Rigidbody2DComponent::Mass)
        .property("GravityScale", &Rigidbody2DComponent::GravityScale);

    ComponentReflection::RegisterComponentType<Rigidbody2DComponent>();
}