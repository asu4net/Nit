#include "DemoGame.h"
#include "Camera/LogicCamera.h"
#include "Camera/ViewportCameraController.h"

using namespace Nit;

void DemoGame::OnInitialize()
{
    Camera = std::make_shared<LogicCamera>(Game::GetInstance().GetWindow());
    Camera->AddController<ViewportCameraController>();

    AssetManager& assetManager = AssetManager::GetInstance();
    GridTexture = assetManager.GetAssetByName<Texture2D>("Grid");
    CatTexture = assetManager.GetAssetByName<Texture2D>("Bola");
    CppTexture = assetManager.GetAssetByName<Texture2D>("Cpp");
    
    Grid.Texture = GridTexture.Lock();
    Grid.Size *= 30;
    Grid.UVScale *= 30;
    Grid.Color = Math::DarkGreyColor;
}

void DemoGame::OnUpdate(const TimeStep& timeStep)
{
    AssetManager& assetManager = AssetManager::GetInstance();
    if (!CatTexture.IsValid())
        CatTexture = assetManager.GetAssetByName<Texture2D>("Bola");
    
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
