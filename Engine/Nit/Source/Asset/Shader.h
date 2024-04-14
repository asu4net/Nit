#pragma once
#include "Asset.h"

namespace Nit
{
    class CShader : public Asset
    {
    public:
        CShader() = default;

        bool Load() override;
        void Unload() override;

        Id GetRendererId() const { return m_RendererShaderId; }

    private:
        Id m_RendererShaderId = 0;
        TString m_VertexSource;
        TString m_FragmentSource;

        bool ReadFromFile(const TString& fileLocation, TString& vertexSource, TString& fragmentSource);
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    namespace ShaderRegistration
    {
        RTTR_REGISTRATION
        {
            rttr::registration::class_<CShader>("Shader")
                .constructor<>();
        }
    }
}
