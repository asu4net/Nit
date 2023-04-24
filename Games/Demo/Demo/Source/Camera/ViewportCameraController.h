#pragma once
#include <ShardGraphics.h>
#include "CameraController.h"

class ViewportCameraController : public CameraController
{
public:
    float MoveSpeed;
    
    ViewportCameraController(Shard::Graphics::Camera* owner);
    void OnUpdate(const float deltaTime) override;
};
