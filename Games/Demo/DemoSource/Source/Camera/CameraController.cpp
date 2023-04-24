#include "CameraController.h"

CameraController::CameraController(Nit::Camera* owner)
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