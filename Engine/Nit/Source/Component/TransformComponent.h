#pragma once
#include "Entity/Entity.h"

namespace Nit
{
    struct TransformComponent
    {
        CVector3 Position;
        CVector3 Rotation;
        CVector3 Scale = CVector3::One;

        TransformComponent() = default;

        TransformComponent(const CVector3& position, const CVector3& rotation, const CVector3& scale)
            : Position(position)
            , Rotation(rotation)
            , Scale(scale)
        {
        }

        CMatrix4 GetMatrix()  const { return CMatrix4::CreateTransform(Position, Rotation, Scale); }
        CVector3 GetUp()      const { return CVector3::LookAt(Rotation, CVector3::Up); }
        CVector3 GetRight()   const { return CVector3::LookAt(Rotation, CVector3::Right); }
        CVector3 GetForward() const { return CVector3::LookAt(Rotation, CVector3::Forward); }
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(TransformComponent)
}