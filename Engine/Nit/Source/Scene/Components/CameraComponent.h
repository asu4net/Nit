#pragma once
#include "ActorComponent.h"
#include "Rendering/Camera.h"

namespace Nit
{
    struct CameraComponent : ActorComponent
    {
        Camera CameraData;
        int Priority = 0;
        
        RTTR_ENABLE(ActorComponent)
    };
    NIT_FORCE_LINK(CameraComponent)

    class CameraStatics
    {
    public:
        static Vec3 ScreenToWorldPoint(const Camera& camera, const Vec2& screenPoint);
    };
}