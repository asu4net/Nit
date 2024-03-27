#pragma once
#include "Asset/AssetRef.h"

namespace Nit
{
    struct MaterialComponent
    {
        AssetRef MaterialRef;

        MaterialComponent() = default;

        MaterialComponent(const AssetRef& materialRef)
            : MaterialRef(materialRef)
        {
        }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(MaterialComponent)
}