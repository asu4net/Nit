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

        Id GetRendererId() const { return m_RendererShaderId; }

    private:
        Id m_RendererShaderId = 0;
        String m_VertexSource;
        String m_FragmentSource;

        bool ReadFromFile(const String& fileLocation, String& vertexSource, String& fragmentSource);
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    namespace ShaderRegistration
    {
        RTTR_REGISTRATION
        {
            rttr::registration::class_<Shader>("Shader")
                .constructor<>();
        }
    }
}
