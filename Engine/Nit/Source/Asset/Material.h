#pragma once
#include "Asset.h"

namespace Nit
{
    class Material : public Asset
    {
    public:
        Material() = default;

        bool Load() override;
        void Unload() override;

    private:

        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    namespace MaterialRegistration
    {
        RTTR_REGISTRATION
        {
            rttr::registration::class_<Material>("Material")
                .constructor<>();
        }
    }
}
