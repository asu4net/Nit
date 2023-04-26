#pragma once
#include <Nit.h>

class ViewportCameraController;
class LogicCamera;

class DemoGame : public Nit::GameLayer
{
public:
    Nit::Shared<LogicCamera> Camera;

    Nit::Shared<Nit::Texture> GridTexture;
    Nit::Shared<Nit::Texture> CatTexture;
    Nit::Shared<Nit::Texture> CppTexture;

    glm::vec3 BallPosition = {.5f, .3f, 0.f};
    Nit::Quad Grid;
    
    void OnInitialize() override;
    void OnUpdate(const Nit::TimeStep& timeStep) override;
};
