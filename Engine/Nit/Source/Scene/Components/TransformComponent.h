#pragma once
#include "ActorComponent.h"

namespace Nit
{
    struct TransformComponent : ActorComponent
    {
        TransformComponent() = default;

        TransformComponent(const Vec3& position, const Quat& rotation = QuatIdentity, const Vec3& scale = VecOne)
            : Position(position)
            , Rotation(rotation)
            , Scale(scale)
        {}
        
        Vec3 Position = VecZero;
        Quat Rotation = QuatIdentity;
        Vec3 Scale = VecOne;
        
        RTTR_ENABLE(ActorComponent)
    };
    NIT_FORCE_LINK(TransformComponent)

    struct TransformStatics
    {
        static void SetFromMat4(TransformComponent& transform, const Mat4& mat);
        static Mat4 GetModelMat4(const TransformComponent& transform);
        static Vec3 GetUpVec3(const TransformComponent& transform);
        static Vec3 GetRightVec3(const TransformComponent& transform);
        static Vec3 GetForwardVec3(const TransformComponent& transform);
        static Vec3 GetEulerAngles(const TransformComponent& transform);
    };
}