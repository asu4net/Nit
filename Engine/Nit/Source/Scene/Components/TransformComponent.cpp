#include "TransformComponent.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace Nit { NIT_FORCE_LINK_IMPL(TransformComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<TransformComponent>("TransformComponent")
        .constructor<>()
        .constructor<const Vec3&, const Quat&, const Vec3&>()
        .property("Position", &TransformComponent::Position)
        .property("Rotation", &TransformComponent::Rotation)
        .property("Scale", &TransformComponent::Scale);
    NIT_REGISTRY_COMPONENT(TransformComponent)
}

namespace Nit
{
    void TransformStatics::SetFromMat4(TransformComponent& transform, const Mat4& mat)
    {
        Math::Decompose(mat, transform.Position, transform.Rotation,
            transform.Scale);
    }

    Mat4 TransformStatics::GetModelMat4(const TransformComponent& transform)
    {
        return Math::Translate(MatIdentity, transform.Position)
                * Math::ToMat4(transform.Rotation)
                * Math::Scale(MatIdentity, transform.Scale);
    }

    Vec3 TransformStatics::GetUpVec3(const TransformComponent& transform)
    {
        return Math::LookAt(transform.Rotation, VecUp);
    }

    Vec3 TransformStatics::GetRightVec3(const TransformComponent& transform)
    {
        return Math::LookAt(transform.Rotation, VecRight);
    }

    Vec3 TransformStatics::GetForwardVec3(const TransformComponent& transform)
    {
        return Math::LookAt(transform.Rotation, VecForward);
    }
}