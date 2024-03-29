#pragma once
#include "Asset/AssetRef.h"
#include "Entity/EntityRef.h"

namespace Nit
{
    struct FamilyComponent
    {
        EntityRef ParentRef;
        EntityRef PrevParentRef;
        
        FamilyComponent() = default;
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(FamilyComponent)
}
