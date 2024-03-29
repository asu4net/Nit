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

        Matrix4 GetMatrix()  const { return Matrix4::CreateTransform(Position, Rotation, Scale); }
        Vector3 GetUp()      const { return Vector3::LookAt(Rotation, Vector3::Up); }
        Vector3 GetRight()   const { return Vector3::LookAt(Rotation, Vector3::Right); }
        Vector3 GetForward() const { return Vector3::LookAt(Rotation, Vector3::Forward); }
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(TransformComponent)
}