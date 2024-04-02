#pragma once
#include "Asset/AssetRef.h"

namespace Nit
{
    struct CircleColliderComponent
    {
        CircleColliderComponent() = default;
        
        float    Radius            = 0.5f;
        Vector2  Center            = Vector2::Zero;
        AssetRef PhysicMaterialRef;
        float    PrevRadius        = 1;
        void*    FixturePtr        = nullptr;
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(CircleColliderComponent)
}