#pragma once
#include "SceneSystem.h"
#include "Core/Asset/AssetLink.h"

namespace Nit
{
    class TextSystem : public SceneSystem
    {
        NIT_EXECUTION_ORDER(700)
    public:
        TextSystem() = default;
        TextSystem(const Weak<Scene>& scene);
        
        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;

        void OnTextComponentAdded(entt::registry&, const entt::entity entity);
    
    private:
        AssetLink<Font> m_DefaultFont;
        RTTR_ENABLE(SceneSystem)
    };
    NIT_FORCE_LINK(TextSystem)
}