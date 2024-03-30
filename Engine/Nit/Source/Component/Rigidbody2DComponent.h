#pragma once

namespace Nit
{
    enum class EBodyType { Static, Kinematic, Dynamic };
    
    struct Rigidbody2DComponent
    {
        EBodyType BodyType     = EBodyType::Dynamic;
        float     Mass         = 1.f;
        float     GravityScale = 1.f;
        void*     BodyPtr      = nullptr;

        EBodyType PrevBodyType = EBodyType::Dynamic;
        
        Rigidbody2DComponent() = default;

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(Rigidbody2DComponent)
}