#pragma once

namespace Nit
{
    struct CameraComponent
    {
        bool IsStartCamera = false;
        CameraProjection Projection = CameraProjection::Perspective;
        float Size = 1.f;
        float Fov = 85.f;
        float NearPlane = 0.1f;
        float FarPlane = 100.f;

        Matrix4 ProjectionMatrix;
        Matrix4 ViewMatrix;

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(CameraComponent)

    class Entity;

    struct CameraStatics 
    {
        static void UpdateCameraMatrices(Entity actor);
        static void UpdateCameraMatrices(CameraComponent& camera, const Vector3& position, const Vector3& rotation);
        static Vector3 ScreenToWorldPoint(CameraComponent& camera, const Vector2& screenPoint);
        static Vector3 ScreenToWorldPoint(Entity actor, const Vector2& screenPoint);
    };
}