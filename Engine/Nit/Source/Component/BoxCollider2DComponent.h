#pragma once
#include "Asset/AssetRef.h"

namespace Nit
{
    struct BoxCollider2DComponent
    {
        BoxCollider2DComponent() = default;
        
        Vector2  Size              = Vector2::One;
        Vector2  Center            = Vector2::Zero;
        AssetRef PhysicMaterialRef;
        Vector2  PrevSize          = Vector2::One;
        void*    FixturePtr        = nullptr;
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(BoxCollider2DComponent)
}