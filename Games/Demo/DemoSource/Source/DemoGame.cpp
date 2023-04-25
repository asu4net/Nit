#include "DemoGame.h"
#include "Camera/LogicCamera.h"
#include "Camera/ViewportCameraController.h"

using namespace Nit;

void DemoGame::OnInitialize()
{
    Camera = std::make_shared<LogicCamera>(Game::GetInstance().GetWindow());
    Camera->AddController<ViewportCameraController>();
    
    GridTexture = Texture2D::Create("Content/Textures/Checkerboard.png", {MagFilter::Nearest});
    CatTexture = Texture2D::Create("Content/Textures/bola.jpg");
    CppTexture = Texture2D::Create("Content/Textures/cpp.png");

    Grid.Texture = GridTexture;
    Grid.Size *= 30;
    Grid.UVScale *= 30;
    Grid.Color = Global::DarkGreyColor;
}

void DemoGame::OnUpdate(const TimeStep& timeStep)
{
    Camera->Update(timeStep.DeltaTime);
    Renderer2D& renderer = Renderer2D::GetInstance();
    
    renderer.SetRenderData({Camera->GetRenderData()});
    renderer.SetBlendingMode(BlendingMode::Alpha);
    renderer.Begin();
    renderer.SubmitQuad(Grid);
    renderer.SubmitQuad({translate(Global::IdentityMatrix, {1, 0, 0}), Global::LightRedColor});
    renderer.SubmitQuad({translate(Global::IdentityMatrix, {0, 1, 0}), Global::YellowColor});
    renderer.SubmitQuad({Global::IdentityMatrix, Global::LightBlueColor});
    renderer.SubmitQuad({translate(Global::IdentityMatrix, {0, -1, 0}), Global::WhiteColor, CppTexture});
    renderer.End();
    renderer.SetBlendingMode(BlendingMode::Add);
    renderer.Begin();
    renderer.SubmitQuad({translate(Global::IdentityMatrix, BallPosition), Global::WhiteColor, CatTexture});
    renderer.End();
}