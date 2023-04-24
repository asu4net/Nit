#include "ViewportCameraController.h"

ViewportCameraController::ViewportCameraController(Shard::Graphics::Camera* owner)
    : CameraController(owner)
    , MoveSpeed(2.f)
{
}

void ViewportCameraController::OnUpdate(const float deltaTime)
{
    using namespace Shard::Graphics;
    
    const float displacement = MoveSpeed * deltaTime;
    glm::vec3& position = GetOwner()->Position;
    
    if (Input::IsKeyPressed(KEY_S))
        position.z -= displacement;
    if (Input::IsKeyPressed(KEY_W))
        position.z += displacement;
            
    if (Input::IsKeyPressed(KEY_A))
        position.x -= displacement;
    if (Input::IsKeyPressed(KEY_D))
        position.x += displacement;

    if (Input::IsKeyPressed(KEY_LEFT_SHIFT))
        position.y -= displacement;
    if (Input::IsKeyPressed(KEY_SPACE))
        position.y += displacement;
}
