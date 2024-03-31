#include "Rigidbody2DComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    Rigidbody2DComponent::Rigidbody2DComponent(const Rigidbody2DComponent& other)
        : IsEnabled(other.IsEnabled)
        , BodyType(other.BodyType)
        , Mass(other.Mass)
        , GravityScale(other.GravityScale)
        , FollowTransform(other.FollowTransform)
        , BodyPtr(nullptr)
        , PrevBodyType(other.PrevBodyType)
    {
    }

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
        .property("IsEnabled",       &Rigidbody2DComponent::IsEnabled)
        .property("BodyType",        &Rigidbody2DComponent::BodyType)
        .property("Mass",            &Rigidbody2DComponent::Mass)
        .property("GravityScale",    &Rigidbody2DComponent::GravityScale)
        .property("FollowTransform", &Rigidbody2DComponent::FollowTransform);

    ComponentReflection::RegisterComponentType<Rigidbody2DComponent>();
}