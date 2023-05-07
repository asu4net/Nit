#include "TheGameLayer.h"
#include "Camera/LogicCamera.h"
#include "Camera/ViewportCameraController.h"

using namespace Nit;

void TheGameLayer::OnInitialize()
{
    Camera = std::make_shared<LogicCamera>(Game::GetInstance().GetWindow());
    Camera->AddController<ViewportCameraController>();

    AssetManager& assetManager = AssetManager::GetInstance();
    GridTexture = assetManager.GetAssetByName<Texture2D>("Grid");
    CatTexture = assetManager.GetAssetByName<Texture2D>("Bola");
    CppTexture = assetManager.GetAssetByName<Texture2D>("Cpp");
    LaserAudio = assetManager.GetAssetByName<AudioBuffer>("laser");
    TheFont = assetManager.GetAssetByName<Font>("CascadiaMono");
    
    AudioManager& audioManager = AudioManager::GetInstance();
    LaserAudioSource = audioManager.CreateSource(LaserAudio.Lock());

    Game::GetInstance().GetWindow()->Events().KeyPressedEvent.Add([&](int key, bool repeat)
    {
        if (key != KEY_SPACE) return;
        audioManager.Play(LaserAudioSource);
    });
    
    Grid.Texture = GridTexture.Lock();
    Grid.Size *= 30;
    Grid.UVScale *= 30;
    Grid.Color = Math::DarkGreyColor;

#ifdef NIT_IMGUI
    ImGuiRenderer::GetInstance().PushWidget<Vector3Widget>(BallPosition, "Ball Pos");
#endif
}

void TheGameLayer::OnUpdate(const TimeStep& timeStep)
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
    glm::mat4 transform = Math::IdentityMatrix;
    transform *= glm::translate(transform, {1, 0, 0});
    transform *= glm::scale(transform,  {5, 5, 1});
    
    renderer.SubmitQuad({translate(Math::IdentityMatrix, {0, 1, 0}), Math::YellowColor});
    renderer.SubmitQuad({Math::IdentityMatrix, Math::LightBlueColor});
    renderer.SubmitQuad({translate(Math::IdentityMatrix, {0, -1, 0}), Math::WhiteColor, CppTexture.Lock()});
    renderer.SubmitQuad({translate(Math::IdentityMatrix, {1, 0, 0}), Math::LightRedColor});
    TextQuad textQuad;
    textQuad.Text = "hello world";
    textQuad.Font = TheFont.Lock();
    textQuad.ModelMatrix = glm::translate(Math::IdentityMatrix, {0, 1.6, 0});
    
    renderer.SubmitTextQuad(textQuad);
    renderer.End();
    renderer.SetBlendingMode(BlendingMode::Add);
    renderer.Begin();
    renderer.SubmitQuad({translate(Math::IdentityMatrix, BallPosition), Math::WhiteColor, CatTexture.Lock()});
    renderer.End();
}

void TheGameLayer::OnFinalize()
{
    AudioManager& audioManager = AudioManager::GetInstance();
    audioManager.DestroySource(LaserAudioSource);
}
