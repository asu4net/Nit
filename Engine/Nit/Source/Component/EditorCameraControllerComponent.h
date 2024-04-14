#pragma once
#include "CameraComponent.h"

namespace Nit
{
    struct EditorCameraControllerComponent
    {
        float ZoomStep = 0.15f;
        float ZoomSpeed = 1.5f;
        float MoveSpeed = 2.f;
        
        // Internal stuff
        float DesiredZoom;
        CVector3 AuxPosition;
        bool MouseDown = false;
        CVector3 OffsetPos;
        bool bIsRightMousePressed;
        CameraComponent AuxCamera;

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(EditorCameraControllerComponent)
}