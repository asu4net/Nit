#pragma once
#include "SceneSystem.h"

namespace Nit
{
    struct TransformComponent;
    struct SpriteComponent;
    struct Quad;
    class Renderer2D;
    
    class SpriteSystem : public SceneSystem
    {
        NIT_EXECUTION_ORDER(600)
    public:
        SpriteSystem() = default;
        SpriteSystem(const Weak<Scene>& scene);
        
        void OnUpdate(const TimeStep& timeStep) override;
        static void SetRendererQuadData(const TransformComponent& transform, const SpriteComponent& sprite, Quad& quad);
    
    private:
        RTTR_ENABLE(SceneSystem)
    };

    NIT_FORCE_LINK(SpriteSystem)
}
