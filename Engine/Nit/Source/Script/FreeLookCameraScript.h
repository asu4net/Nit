#pragma once
#include "Script/Script.h"

namespace Nit
{
    class FreeLookCameraScript : public Script
    {
    public:
        float ZoomStep = 0.15f;
        float ZoomSpeed = 1.5f;
        float MoveSpeed = 2.f;
        bool CanMove = true;
        
        void OnStart() override;
        void OnUpdate() override;

    private:
        void UpdateCameraZoom();
        void UpdateCameraPosition();
        
        float m_DesiredZoom;
        Vector3 m_AuxPosition;
        Matrix4 m_AuxCameraMatrix;
        bool m_bMouseDown = false;
        Vector3 m_Offset;
    
        RTTR_ENABLE(Script)
    };

    NIT_FORCE_LINK(FreeLookCameraScript);
}