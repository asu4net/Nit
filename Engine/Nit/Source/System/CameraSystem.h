#pragma once
#include "Entity/Entity.h"

namespace Nit::CameraSystem
{
    void Register();
    void SetMainCameraEntity(Entity entity);
    Entity GetMainCameraEntity();
    void OnUpdate();
    void OnStart();
}