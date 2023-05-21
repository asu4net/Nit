#include "Game.h"

using namespace Nit;

void Game::OnStart()
{
    AssetManager& assets = AssetManager::GetInstance();
    Scene& scene = World::GetActiveScene();

    // Game camera
    const Actor cameraActor = scene.CreateActor("GameCamera");
    cameraActor.Add<CameraComponent>();
    auto& camTransform = cameraActor.Get<TransformComponent>();
    camTransform.Position = VecForward * -2.f;

    // Laser audio
    const Actor laserAudioActor = scene.CreateActor("LaserAudio");
    const auto& audioSourceLaser = laserAudioActor.Add<AudioSourceComponent>(assets.GetAssetByName<AudioClip>("laser"));
    
    Engine::GetInstance().GetWindow()->Events().KeyPressedEvent.Add([&](const int key, bool){
         if (key != KEY_SPACE) return;
         AudioStatics::Play(audioSourceLaser);
    });

    // Hello World text
    const Actor helloWorldText = scene.CreateActor("HelloWorld", {0, 1.6, 0});
    auto& text = helloWorldText.Add<TextComponent>("hello world");
    text.Size = { 2, 2 };

    // Colored sprites
    scene.CreateActor("BlueSprite").Add<SpriteComponent>(LightBlue);
    scene.CreateActor("YellowSprite", VecUp).Add<SpriteComponent>(Yellow);
    scene.CreateActor("RedSprite", VecRight).Add<SpriteComponent>(LightRed);

    // Cat sprite
    const Actor catActor = scene.CreateActor("Cat", VecLeft);
    catActor.Add<SpriteComponent>(assets.GetAssetByName<Texture2D>("Bola"));

    //C++ sprite
    scene.CreateActor("CppTexture", VecDown).Add<SpriteComponent>(assets.GetAssetByName<Texture2D>("Cpp"));

    // Background Grid
    const Actor gridActor = scene.CreateActor();
    auto& gridSprite = gridActor.Add<SpriteComponent>(assets.GetAssetByName<Texture2D>("Grid"));
    gridSprite.Size *= 30;
    gridSprite.UVScale *= 30;
    gridSprite.Color = DarkGrey;

    // Debug window
#ifdef NIT_IMGUI
    ImGuiRenderer::GetInstance().PushWidget<Vector3Widget>(catActor.Get<TransformComponent>().Position, "Ball Pos");
#endif

    //World::GetActiveScene().SetRuntimeEnabled(true);
}