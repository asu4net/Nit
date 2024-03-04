#pragma once
#include "Entity/Entity.h"

namespace Nit
{
    struct TransformComponent
    {
        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale = Vector3::One;

        TransformComponent() = default;

        TransformComponent(const Vector3& position, const Vector3& rotation, const Vector3& scale)
            : Position(position)
            , Rotation(rotation)
            , Scale(scale)
        {
        }

        Matrix4 GetMatrix() const { return Matrix4::CreateTransform(Position, Rotation, Scale); }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(TransformComponent)
}