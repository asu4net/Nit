#pragma once
#include "Rendering/Camera.h"
#include "Scene/Components/ActorComponent.h"

namespace Nit
{
    struct EditorCameraComponent : ActorComponent
    {
        Camera CameraData;
        float MoveSpeed = 2.f;
        float ZoomStep = 0.25f;
        bool bCanMove = true;
        
        RTTR_ENABLE(ActorComponent)
    };
    NIT_FORCE_LINK(EditorCameraComponent)
}