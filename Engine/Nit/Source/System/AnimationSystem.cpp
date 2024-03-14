#include "AnimationSystem.h"
#include "Asset/Animation.h"
#include "Asset/Scene.h"
#include "Component/AnimationComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/Engine.h"

namespace Nit::AnimationSystem
{
    const String SystemID = "AnimationSystem";

    void Register()
    {
        Engine::CreateSystem(SystemID, AnimationExecutionOrder, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
    }

    void OnCreate()
    {
        World::GetRegistry().on_construct<AnimationComponent>().connect<&OnAnimationComponentAdded>();
    }
    
    void OnAnimationComponentAdded(Registry&, const RawEntity entity)
    {
        Entity animationActor = entity;
        AnimationComponent& animationComponent = animationActor.Get<AnimationComponent>();
        animationComponent.CurrentAnimation.Retarget();
    }

    void OnStart()
    {
        const auto view = World::GetRegistry().view<AnimationComponent>();

        view.each([&](RawEntity entity, AnimationComponent& animationComponent) {

            if (!animationComponent.PlayOnStart || !animationComponent.CurrentAnimation.IsValid())
            {
                return;
            }

            animationComponent.IsPlaying = true;
        });
    }

    void OnUpdate()
    {
        const auto view = World::GetRegistry().view<AnimationComponent>();

        view.each([&](RawEntity rawEntity, AnimationComponent& animationComponent) {

            if (!animationComponent.IsPlaying || !animationComponent.CurrentAnimation.IsValid())
            {
                return;
            }

            Animation& animation = animationComponent.CurrentAnimation.As<Animation>();

            if (!animation.GetSize())
            {
                return;
            }

            animationComponent.CurrentTime += Time::GetDeltaTime();

            const Animation::Key& currentKey = animation.GetKey(animationComponent.CurrentIndex);

            if (animationComponent.CurrentTime >= currentKey.KeyTime)
            {
                Entity entity = rawEntity;

                if (entity.Has<SpriteComponent>())
                {
                    auto& spriteComponent = entity.Get<SpriteComponent>();
                    spriteComponent.SubSpriteName = currentKey.KeyRef;
                }
                animationComponent.CurrentIndex++;
            }

            if (animationComponent.CurrentIndex == animation.GetSize())
            {
                animationComponent.CurrentTime = 0;
                animationComponent.CurrentIndex = 0;

                if (!animationComponent.LoopEnabled)
                {
                    animationComponent.IsPlaying = false;
                }
            }
        });
    }
}