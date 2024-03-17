#pragma once
#include "Asset.h"
#include "AssetRef.h"

namespace Nit
{
    class Material : public Asset
    {
    public:
        Material() = default;

        AssetRef GetShaderRef() const { return m_ShaderRef; }

        bool Load() override;
        void Unload() override;

    private:
        AssetRef m_ShaderRef = Id(0);

        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };
    NIT_FORCE_LINK(Material)
}
