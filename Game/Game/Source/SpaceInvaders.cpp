#include "SpaceInvaders.h"
#include "Nit.h"
#include "System/AudioSystem.h"
#include "Input/InputModifiers.h"

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
    InputAction* MoveAction;
    InputModifierScalar* ScalarInputModifier;
    InputModifierNegate* NegateInputModifier;
    Entity MissileTemplate;
    DynamicArray<Entity> MissilePool;
    DynamicArray<Entity> FiredMissiles;
    WeakPtr<AudioClip> MissileClip;
    
    DynamicArray<AudioSource> AudioSources;
    DynamicArray<AudioSource> PlayingAudioSources;
    
    int LastMissileIdx = 0;

    bool bIsFireRumbleOn = false;
    float RumbleTimer = 0.f;
    const float RumbleTime = 0.1f;
    const float RumbleForce = 0.5f;
    
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
        
        AudioSystem::Play(source);
        PlayingAudioSources.push_back(source);

        bIsFireRumbleOn = true;
        RumbleTimer = 0.f;
        InputSystem::SendForceFeedback(0, 0.f, RumbleForce);
    }


    void OnCreate()
    {
        MissileClip = Content::GetAssetByName("laser").GetWeakAs<AudioClip>();
        
        LinePrimitive* line = Renderer::CreatePrimitive<LinePrimitive>();
        line->bIsVisible = true;
        line->TintColor = Color::Red;
        line->Start = Vector2::Right;
        line->End = line->Start + Vector2(0.7, 0.2);
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
        
        Player = World::FindEntityByName("SpaceShip");

        ShootAction = InputSystem::CreateInputAction(WiimoteKeyNames::Wiimote_Button_A);
        ShootAction->OnPerformed().Add([](const InputActionContext& context) {
            if (Engine::IsPaused()) return;
            FireMissile();
        });

        MoveAction = InputSystem::CreateInputAction(WiimoteKeyNames::Wiimote_Pointer2D);
        MoveAction->OnPerformed().Add([](const InputActionContext& context) {
            if (Engine::IsPaused()) return;
            Player.GetTransform().Position.x = context.inputValue.x - 4.5f;
            Player.GetTransform().Position.y = context.inputValue.y + 2.5f;
        });

        ScalarInputModifier = new InputModifierScalar(8.9f, 5.f);
        NegateInputModifier = new InputModifierNegate(false, true);

        MoveAction->AddInputModifier(ScalarInputModifier);
        MoveAction->AddInputModifier(NegateInputModifier);

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


        if(bIsFireRumbleOn)
        {
            RumbleTimer += Time::GetDeltaTime();
            if(RumbleTimer >= RumbleTime)
            {
                bIsFireRumbleOn = false;
                RumbleTimer = 0.f;
                InputSystem::SendForceFeedback(0, 0.f, 0.f);
            }
        }
    }

    void OnFinish()
    {
        MissilePool.clear();
        FiredMissiles.clear();

        if (ShootAction)
            InputSystem::DestroyInputAction(ShootAction);
        if (MoveAction)
            InputSystem::DestroyInputAction(MoveAction);
        if (NegateInputModifier)
            delete NegateInputModifier;
        if (ScalarInputModifier)
            delete ScalarInputModifier;

        LastMissileIdx = 0;
    }
}
