#include "Game.h"
#include "Camera/LogicCamera.h"
#include "Camera/ViewportCameraController.h"

using namespace Nit;

void Game::OnStart()
{
    Camera = std::make_shared<LogicCamera>(Engine::GetInstance().GetWindow());
    Camera->AddController<ViewportCameraController>();

    AssetManager& assetManager = AssetManager::GetInstance();
    GridTexture = assetManager.GetAssetByName<Texture2D>("Grid");
    CatTexture = assetManager.GetAssetByName<Texture2D>("Bola");
    CppTexture = assetManager.GetAssetByName<Texture2D>("Cpp");
    LaserAudio = assetManager.GetAssetByName<AudioBuffer>("laser");
    TheFont = assetManager.GetAssetByName<Font>("CascadiaMono");
    
    AudioManager& audioManager = AudioManager::GetInstance();
    LaserAudioSource = audioManager.CreateSource(LaserAudio.Lock());

    Engine::GetInstance().GetWindow()->Events().KeyPressedEvent.Add([&](int key, bool repeat)
    {
        if (key != KEY_SPACE) return;
        audioManager.Play(LaserAudioSource);
    });
    
    Grid.Texture = GridTexture.Lock();
    Grid.Size *= 30;
    Grid.UVScale *= 30;
    Grid.Color = DarkGrey;

    HelloWorldMessage.Text = "hello world";
    HelloWorldMessage.Font = TheFont.Lock();
    HelloWorldMessage.ModelMatrix = glm::translate(MatIdentity, {0, 1.6, 0});
    HelloWorldMessage.Spacing = 0.9f;
    HelloWorldMessage.Size = { 2, 2 };

#ifdef NIT_IMGUI
    ImGuiRenderer::GetInstance().PushWidget<Vector3Widget>(BallPosition, "Ball Pos");
#endif
}

void Game::OnUpdate(const TimeStep& timeStep)
{
    AssetManager& assetManager = AssetManager::GetInstance();
    if (!CatTexture.IsValid())
        CatTexture = assetManager.GetAssetByName<Texture2D>("Bola");
    
    Camera->Update(timeStep.DeltaTime);
    Renderer2D& renderer = Renderer2D::GetInstance();

    renderer.ClearScreen(DarkGrey);
    renderer.SetRenderData({Camera->GetRenderData()});
    renderer.SetBlendingMode(BlendingMode::Alpha);
    renderer.Begin();
    renderer.SubmitQuad(Grid);
    renderer.SubmitQuad({translate(MatIdentity, {0, 1, 0}), Yellow});
    renderer.SubmitQuad({MatIdentity, LightBlue});
    renderer.SubmitQuad({translate(MatIdentity, {0, -1, 0}), White, CppTexture.Lock()});
    renderer.SubmitQuad({translate(MatIdentity, {1, 0, 0}), LightRed});
    renderer.SubmitTextQuad(HelloWorldMessage);
    renderer.End();
    renderer.SetBlendingMode(BlendingMode::Add);
    renderer.Begin();
    renderer.SubmitQuad({translate(MatIdentity, BallPosition), White, CatTexture.Lock()});
    renderer.End();
}

void Game::OnFinish()
{
    AudioManager& audioManager = AudioManager::GetInstance();
    audioManager.DestroySource(LaserAudioSource);
}
