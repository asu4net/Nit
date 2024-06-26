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
        .property("Projection", &CameraComponent::Projection)
        .property("Size", &CameraComponent::Size)
        .property("Fov", &CameraComponent::Fov)
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
        
    void CameraStatics::UpdateCameraMatrices(CameraComponent& camera, const Vector3& position, const Vector3& rotation)
    {
        if (camera.Projection == CameraProjection::Orthographic)
        {
            camera.ProjectionMatrix = Matrix4::OrthoProjection(Engine::GetScreenAspect(), camera.Size, camera.NearPlane, camera.FarPlane);
        }
        else if (camera.Projection == CameraProjection::Perspective)
        {
            camera.ProjectionMatrix = Matrix4::PerspectiveProjection(Engine::GetScreenAspect(), camera.Fov, camera.NearPlane, camera.FarPlane);
        }
        
        camera.ViewMatrix = Matrix4::ViewProjection(position, rotation);
    }

    Vector3 CameraStatics::ScreenToWorldPoint(Entity actor, const Vector2& screenPoint)
    {
        CameraComponent& cameraComponent = actor.Get<CameraComponent>();
        return Math::ScreenToWorldPoint(cameraComponent.ProjectionMatrix * cameraComponent.ViewMatrix, screenPoint, Engine::GetScreenSize());
    }

    Vector3 CameraStatics::ScreenToWorldPoint(CameraComponent& camera, const Vector2& screenPoint)
    {
        return Math::ScreenToWorldPoint(camera.ProjectionMatrix * camera.ViewMatrix, screenPoint, Engine::GetScreenSize());
    }
}