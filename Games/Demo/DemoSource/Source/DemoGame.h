#pragma once
#include <Nit.h>

class ViewportCameraController;
class LogicCamera;

class DemoGame : public Nit::GameLayer
{
public:
    std::shared_ptr<LogicCamera> Camera;
    
    std::shared_ptr<Nit::Texture> GridTexture;
    std::shared_ptr<Nit::Texture> CatTexture;
    std::shared_ptr<Nit::Texture> CppTexture;

    glm::vec3 BallPosition = {.5f, .3f, 0.f};
    Nit::Quad Grid;
    
    void OnInitialize() override;
    void OnUpdate(const Nit::TimeStep& timeStep) override;
};
