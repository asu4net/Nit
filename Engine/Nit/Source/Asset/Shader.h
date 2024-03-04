#pragma once
#include "Asset.h"

namespace Nit
{
    class Shader : public Asset
    {
    public:
        Shader() = default;

        bool Load() override;
        void Unload() override;

    private:
        Id m_RendererShaderId = 0;
        String m_VertexSource;
        String m_FragmentSource;

        bool ReadFromFile(const String& fileLocation, String& vertexSource, String& fragmentSource);
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    YDECLARE_ASSET_EXTENSION(Shader, glsl)
}
