#include "SpaceInvaders.h"
#include "Nit.h"

namespace Nit::SpaceInvaders
{
    void OnStart();
    void OnUpdate();
    void OnFinish();

    void Register()
    {
        Engine::CreateSystem("Space Invaders", 5000, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::Finish, &OnFinish);
    }

    uint32_t NumOfMissiles = 20;
    constexpr float PlayerMoveSpeed = 3.f;
    constexpr float MissileMoveSpeed = 5.f;
    
    Entity Player;
    InputAction* ShootAction;
    Entity MissileTemplate;
    DynamicArray<Entity> MissilePool;
    DynamicArray<Entity> FiredMissiles;
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
    }

    void OnStart()
    {
        ShootAction = InputSystem::CreateInputAction(Key_Space);
        ShootAction->OnPerformed().Add([] (const InputActionContext& context){
            if (Engine::IsPaused() || context.IsReleased) return;
            FireMissile();
        });

        Player = World::FindEntityByName("SpaceShip");
        MissileTemplate = World::FindEntityByName("MissileTemplate");

        MissilePool.reserve(NumOfMissiles);
        for (int i = 0; i < NumOfMissiles; ++i)
        {
            SpawnMissile();
        }
    }

    void OnUpdate()
    {
        const int left  = Input::IsKeyPressed(Key_A) ? -1 : 0;
        const int right = Input::IsKeyPressed(Key_D) ?  1 : 0;
        const int dir = left + right;
        Player.GetTransform().Position += Vector2::Right * dir * PlayerMoveSpeed * Time::GetDeltaTime();

        for (Entity missile : FiredMissiles)
        {
            missile.GetTransform().Position += Vector2::Up * MissileMoveSpeed * Time::GetDeltaTime();
        }
    }

    void OnFinish()
    {
        MissilePool.clear();
        FiredMissiles.clear();

        InputSystem::DestroyInputAction(ShootAction);
        LastMissileIdx = 0;
    }
}