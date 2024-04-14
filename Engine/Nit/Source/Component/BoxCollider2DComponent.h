#pragma once
#include "Asset/AssetRef.h"

namespace Nit
{
    struct BoxCollider2DComponent
    {
        BoxCollider2DComponent() = default;
        
        CVector2  Size              = CVector2::One;
        CVector2  Center            = CVector2::Zero;
        AssetRef PhysicMaterialRef;
        CVector2  PrevSize          = CVector2::One;
        void*    FixturePtr        = nullptr;
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(BoxCollider2DComponent)
}