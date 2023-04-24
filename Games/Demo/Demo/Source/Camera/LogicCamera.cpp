#include "LogicCamera.h"

LogicCamera::LogicCamera(const std::shared_ptr<Shard::Graphics::Window>& window,
                         const Projection startProjection)
    : Camera(startProjection)
{
    window->Events().ResizeEvent.Add([&](int width, int height)
    {
        AspectRatio = window->GetAspect();
    });
}

LogicCamera::~LogicCamera()
{
    for (const CameraController* controller : m_Controllers)
        delete controller;
}

void LogicCamera::Update(const float deltaTime)
{
    UpdateMatrix();
    if (CameraController* controller = GetActiveController())
        controller->Update(deltaTime);
}

Shard::Graphics::Camera LogicCamera::GetRenderData()
{
    return static_cast<Camera>(*this);
}
