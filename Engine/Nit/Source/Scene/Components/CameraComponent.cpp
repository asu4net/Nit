#include "CameraComponent.h"

#include "Core/Engine.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Window/Window.h"

namespace Nit { NIT_FORCE_LINK_IMPL(CameraComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::class_<CameraComponent>("CameraComponent")
        .constructor<>()
        .property("CameraData", &CameraComponent::CameraData)
        .property("Priority", &CameraComponent::Priority);

    NIT_REGISTRY_COMPONENT(CameraComponent)
}

namespace Nit
{
    Vec3 CameraStatics::ScreenToWorldPoint(const Camera& camera, const Vec2& screenPoint)
    {
        const Engine& engine = Engine::GetInstance();
        const float halfScreenWidth = static_cast<float>(engine.GetScreenWidth()) / 2.f;
        const float halfScreenHeight = static_cast<float>(engine.GetScreenHeight()) / 2.f;
        const glm::mat4 inverseMv = glm::inverse(camera.GetProjectionViewMat4());
        const glm::vec4 nearPlane = glm::vec4(((screenPoint.x - halfScreenWidth) / halfScreenWidth),
            (-1 * (screenPoint.y - halfScreenHeight) / halfScreenHeight), -1, 1.0);
        glm::vec4 nearResult = inverseMv * nearPlane;
        nearResult /= nearResult.w;
        return {nearResult.x, nearResult.y, 0};
    }
}