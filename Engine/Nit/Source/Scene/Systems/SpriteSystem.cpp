#include "SpriteSystem.h"
#include "CameraSystem.h"
#include "Scene/Components/SpriteComponent.h"
#include "Scene/Components/TransformComponent.h"

namespace Nit { NIT_FORCE_LINK_IMPL(SpriteSystem) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<SpriteSystem>("SpriteSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&>();
}

namespace Nit
{
    SpriteSystem::SpriteSystem(const Weak<Scene>& scene)
            : SceneSystem(scene)
    {}

    void SpriteSystem::OnUpdate(const TimeStep& timeStep)
    {
        if (!CameraSystem::GetMainCameraActor().IsValid()) return;
        
        const auto view = Registry().view<TransformComponent, SpriteComponent>();
        
        view.each([&](const TransformComponent& transform, const SpriteComponent& sprite){
            Quad spriteQuad;
            SetRendererQuadData(transform, sprite, spriteQuad);
            GetScene().GetSceneRenderer().PushQuad(spriteQuad);
        });
    }

    void SpriteSystem::SetRendererQuadData(const TransformComponent& transform, const SpriteComponent& sprite, Quad& quad)
    {
        quad = {
                TransformStatics::GetModelMat4(transform),
                sprite.Color,
                sprite.TextureLink.IsValid() ? sprite.TextureLink.Lock() : nullptr,
                sprite.bIsSubTexture,
                sprite.SubTextureSize,
                sprite.LocationInAtlas,
                sprite.UVScale,
                sprite.Size,
                sprite.Flip
        };
    }
}