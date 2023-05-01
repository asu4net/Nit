#include "DemoGame.h"
#include "Camera/LogicCamera.h"
#include "Camera/ViewportCameraController.h"

using namespace Nit;

void DemoGame::OnInitialize()
{
    Camera = std::make_shared<LogicCamera>(Game::GetInstance().GetWindow());
    Camera->AddController<ViewportCameraController>();

    AssetManager& assetManager = AssetManager::GetInstance();

    Texture2DSettings gridSettings;
    gridSettings.MagFilter = MagFilter::Nearest;
    GridTexture = assetManager.CreateAsset<Texture2D>("Grid", "Content/Textures/Checkerboard.png",
        gridSettings);
    CatTexture = assetManager.CreateAsset<Texture2D>("Bola", "Content/Textures/bola.jpg");
    CppTexture = assetManager.CreateAsset<Texture2D>("Cpp", "Content/Textures/cpp.png");

    Grid.Texture = GridTexture.Lock();
    Grid.Size *= 30;
    Grid.UVScale *= 30;
    Grid.Color = Math::DarkGreyColor;
}

void DemoGame::OnUpdate(const TimeStep& timeStep)
{
    Camera->Update(timeStep.DeltaTime);
    Renderer2D& renderer = Renderer2D::GetInstance();

    renderer.SetRenderData({Camera->GetRenderData()});
    renderer.SetBlendingMode(BlendingMode::Alpha);
    renderer.Begin();
    renderer.SubmitQuad(Grid);
    renderer.SubmitQuad({translate(Math::IdentityMatrix, {1, 0, 0}), Math::LightRedColor});
    renderer.SubmitQuad({translate(Math::IdentityMatrix, {0, 1, 0}), Math::YellowColor});
    renderer.SubmitQuad({Math::IdentityMatrix, Math::LightBlueColor});
    renderer.SubmitQuad({translate(Math::IdentityMatrix, {0, -1, 0}), Math::WhiteColor, CppTexture.Lock()});
    renderer.End();
    renderer.SetBlendingMode(BlendingMode::Add);
    renderer.Begin();
    renderer.SubmitQuad({translate(Math::IdentityMatrix, BallPosition), Math::WhiteColor, CatTexture.Lock()});
    renderer.End();
}
