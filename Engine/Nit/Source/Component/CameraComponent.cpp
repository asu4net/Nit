#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Entity/Entity.h"
#include "Window/Window.h"
#include "Core/Engine.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(CameraComponent)
}

RTTR_REGISTRATION
{
    using namespace Nit;
    
    rttr::registration::class_<CameraComponent>("CameraComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("IsStartCamera", &CameraComponent::IsStartCamera)
        .property("Size", &CameraComponent::Size)
        .property("NearPlane", &CameraComponent::NearPlane)
        .property("FarPlane", &CameraComponent::FarPlane);

    ComponentReflection::RegisterComponentType<CameraComponent>();
}

namespace Nit
{
    void CameraStatics::UpdateCameraMatrices(Entity actor)
    {
        CameraComponent& camera = actor.Get<CameraComponent>();
        UpdateCameraMatrices(camera, actor.GetTransform().Position, actor.GetTransform().Rotation);
    }
        
    void CameraStatics::UpdateCameraMatrices(CameraComponent& camera, const CVector3& position, const CVector3& rotation)
    {
        camera.ProjectionMatrix = CMatrix4::OrthoProjection(Engine::GetScreenAspect(), camera.Size, camera.NearPlane, camera.FarPlane);
        camera.ViewMatrix = CMatrix4::ViewProjection(position, rotation);
    }

    CVector3 CameraStatics::ScreenToWorldPoint(Entity actor, const CVector2& screenPoint)
    {
        CameraComponent& cameraComponent = actor.Get<CameraComponent>();
        return Math::ScreenToWorldPoint(cameraComponent.ProjectionMatrix * cameraComponent.ViewMatrix, screenPoint, Engine::GetScreenSize());
    }

    CVector3 CameraStatics::ScreenToWorldPoint(CameraComponent& camera, const CVector2& screenPoint)
    {
        return Math::ScreenToWorldPoint(camera.ProjectionMatrix * camera.ViewMatrix, screenPoint, Engine::GetScreenSize());
    }
}