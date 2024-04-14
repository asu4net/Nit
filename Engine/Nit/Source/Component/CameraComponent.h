#pragma once

namespace Nit
{
    struct CameraComponent
    {
        bool IsStartCamera = false;
        float Size = 1.f;
        float NearPlane = 0.f;
        float FarPlane = 1000.f;
        CMatrix4 ProjectionMatrix;
        CMatrix4 ViewMatrix;

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(CameraComponent)

    class Entity;

    struct CameraStatics 
    {
        static void UpdateCameraMatrices(Entity actor);
        static void UpdateCameraMatrices(CameraComponent& camera, const CVector3& position, const CVector3& rotation);
        static CVector3 ScreenToWorldPoint(CameraComponent& camera, const CVector2& screenPoint);
        static CVector3 ScreenToWorldPoint(Entity actor, const CVector2& screenPoint);
    };
}