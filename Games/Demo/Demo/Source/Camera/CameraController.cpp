#include "CameraController.h"

CameraController::CameraController(Shard::Graphics::Camera* owner)
    : m_Owner(owner)
    , m_bEnabled(false)
{
}

void CameraController::Update(const float deltaTime)
{
    if (!m_bEnabled) return;
    OnUpdate(deltaTime);
}

void CameraController::OnUpdate(const float deltaTime) {}