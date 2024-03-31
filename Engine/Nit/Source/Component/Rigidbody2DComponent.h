#pragma once

namespace Nit
{
    enum class EBodyType { Static, Kinematic, Dynamic };
    
    struct Rigidbody2DComponent
    {
        bool      IsEnabled          = true;
        EBodyType BodyType           = EBodyType::Dynamic;
        float     Mass               = 1.f;
        float     GravityScale       = 1.f;
        bool      FollowTransform    = false;
        void*     BodyPtr            = nullptr;
        EBodyType PrevBodyType       = EBodyType::Dynamic;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent& other);
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(Rigidbody2DComponent)
}