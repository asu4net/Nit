#include "CameraSystem.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/TransformComponent.h"

namespace Nit { NIT_FORCE_LINK_IMPL(CameraSystem) }

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;

    registration::class_<CameraSystem>("CameraSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&>();
}

namespace Nit
{
    CameraSystem::CameraSystem(const Weak<Scene>& scene)
        : SceneSystem(scene)
    {
    }
    
    void CameraSystem::OnUpdate(const TimeStep& timeStep)
    {
        const auto view = Registry().view<TransformComponent, CameraComponent>();
        Registry().sort<CameraComponent>([](const CameraComponent& a, const CameraComponent& b)
        {
           return a.Priority > b.Priority;
        });
        
        m_MainCameraActor = { view.front(), RegistryPtr() };
        
        view.each([&](const TransformComponent& transform, CameraComponent& camera)
        {
            camera.CameraData.CalculateProjectionViewMat4(transform.Position, transform.Rotation);
            GetScene().GetSceneRenderer().PushCamera(camera.CameraData);
        });
    }
}