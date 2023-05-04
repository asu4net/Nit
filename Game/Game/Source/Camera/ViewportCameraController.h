#pragma once
#include <Nit.h>
#include "CameraController.h"

class ViewportCameraController : public CameraController
{
public:
    float MoveSpeed;
    
    ViewportCameraController(Nit::Camera* owner);
    void OnUpdate(const float deltaTime) override;
};
