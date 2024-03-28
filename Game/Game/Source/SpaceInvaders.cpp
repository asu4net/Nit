#include "SpaceInvaders.h"
#include "Nit.h"
#include "System/AudioSystem.h"

namespace Nit::SpaceInvaders
{
    void OnCreate();
    void OnStart();
    void OnUpdate();
    void OnFinish();
    
    void Register()
    {
        Engine::CreateSystem("Space Invaders", 8000, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::Finish, &OnFinish);
    }

    uint32_t NumOfMissiles = 20;
    constexpr float PlayerMoveSpeed = 3.f;
    constexpr float MissileMoveSpeed = 5.f;
    const String SpaceInvadersSceneName = "SpaceInvaders";
    
    Entity Player;
    InputAction* ShootAction;
    Entity MissileTemplate;
    DynamicArray<Entity> MissilePool;
    DynamicArray<Entity> FiredMissiles;
    WeakPtr<AudioClip> MissileClip;
    
    DynamicArray<AudioSource> AudioSources;
    DynamicArray<AudioSource> PlayingAudioSources;
    
    int LastMissileIdx = 0;
    
    void SpawnMissile()
    {
        StringStream missileName;
        missileName << "Missile " << MissilePool.size();
        Entity missile = World::CloneEntity(MissileTemplate, missileName.str());
        auto& missileSprite = missile.Get<SpriteComponent>();
        MissilePool.push_back(missile);
    }

    void FireMissile()
    {
        if (LastMissileIdx == NumOfMissiles)
        {
            SpawnMissile();
            NumOfMissiles++;
            LastMissileIdx = NumOfMissiles - 1;
        }

        Entity missile = MissilePool[LastMissileIdx];
        LastMissileIdx++;
        FiredMissiles.push_back(missile);
        missile.GetTransform().Position = Player.GetTransform().Position;
        missile.Get<SpriteComponent>().IsVisible = true;

        // Audio management
        
        if (MissileClip.expired())
        {
            return;
        }

        AudioSource source;
        
        if (AudioSources.empty())
        {
            source = AudioSystem::CreateSource(MissileClip.lock());
        }
        else
        {
            source = AudioSources.back();
            AudioSources.erase(AudioSources.end() - 1);
        }

        AudioSystem::SetPitch(source, 1);
        AudioSystem::SetGain(source, 1);
        AudioSystem::SetLoop(source, false);
        AudioSystem::Play(source);

        PlayingAudioSources.push_back(source);
    }


    void OnCreate()
    {
        MissileClip = Content::GetAssetByName("laser").GetWeakAs<AudioClip>();
    }
    
    void OnStart()
    {
        // stress test
        // for (uint32_t i = 0; i < 5000; i++)
        // {
        //     Entity entity = World::CreateEntity();
        //     auto& sprite = entity.Add<SpriteComponent>();
        //     sprite.SpriteAssetRef = Content::GetAssetByName("Bola");
        // }
        
        if (!World::IsSceneOpened(SpaceInvadersSceneName))
        {
            return;
        }
        
        ShootAction = InputSystem::CreateInputAction(Key_Space);
        ShootAction->OnPerformed().Add([] (const InputActionContext& context){
            if (Engine::IsPaused() || context.IsReleased) return;
            FireMissile();
        });

        Player = World::FindEntityByName("SpaceShip");
        MissileTemplate = World::FindEntityByName("MissileTemplate");

        MissilePool.reserve(NumOfMissiles);
        for (uint32_t i = 0; i < NumOfMissiles; ++i)
        {
            SpawnMissile();
        }
    }

    void OnUpdate()
    {
        if (!World::IsSceneOpened(SpaceInvadersSceneName))
        {
            return;
        }
        
        const int left  = Input::IsKeyPressed(Key_A) ? -1 : 0;
        const int right = Input::IsKeyPressed(Key_D) ?  1 : 0;
        const float dir = (float) (left + right);
        Player.GetTransform().Position += Vector2::Right * dir * PlayerMoveSpeed * Time::GetDeltaTime();

        for (Entity missile : FiredMissiles)
        {
            missile.GetTransform().Position += Vector2::Up * MissileMoveSpeed * Time::GetDeltaTime();
        }
        
        PlayingAudioSources.erase(std::remove_if(PlayingAudioSources.begin(), PlayingAudioSources.end(),
            [](AudioSource& audio)
            {
                const bool bHasFinished = !AudioSystem::IsPlaying(audio);

                if (bHasFinished)
                {
                    AudioSources.push_back(audio);    
                }
                return bHasFinished;
            }),
            PlayingAudioSources.end());
    }

    void OnFinish()
    {
        MissilePool.clear();
        FiredMissiles.clear();

        if (ShootAction)
            InputSystem::DestroyInputAction(ShootAction);

        LastMissileIdx = 0;
    }
}
