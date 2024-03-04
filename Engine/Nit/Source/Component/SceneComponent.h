#pragma once
#include "Asset\AssetRef.h"

namespace Nit
{
    struct SceneComponent
    {
        AssetRef OwnerScene;
        bool IsSerializable;

        SceneComponent() = default;

        SceneComponent(const AssetRef& ownerScene, bool isSerializable)
            : OwnerScene(ownerScene)
            , IsSerializable(isSerializable)
        {}

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(SceneComponent)
}