#pragma once

namespace Nit
{
    struct Component
    {
    };
    
    struct TransformComponent : Component
    {
        TransformComponent() = default;

        TransformComponent(const vec3& position, const quat& rotation = QuatIdentity, const vec3& scale = VecOne)
            : Position(position)
            , Rotation(rotation)
            , Scale(scale)
        {}
        
        vec3 Position = VecZero;
        quat Rotation = QuatIdentity;
        vec3 Scale = VecOne;
    };
}