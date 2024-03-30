#pragma once
#include "Asset.h"

namespace Nit
{
    class PhysicMaterial : public Asset
    {
    public:
        PhysicMaterial() = default;

        float Density             = 1.0f;
        float Friction            = 0.5f;
        float Bounciness          = 0.0f;
        float BouncinessThreshold = 0.5f;
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    NIT_FORCE_LINK(PhysicMaterial)
}