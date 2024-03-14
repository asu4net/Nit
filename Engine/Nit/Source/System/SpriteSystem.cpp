#include "SpriteSystem.h"
#include "Core\Engine.h"
#include "Render\Renderer.h"
#include "Render\SpritePrimitive.h"
#include "Component\SpriteComponent.h"
#include "Component\TransformComponent.h"


namespace Nit::SpriteSystem
{
    const String SystemID = "SpriteSystem";

    void Register()
    {
        Engine::CreateSystem(SystemID, SpriteExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
    }

    void OnCreate()
    {
        World::GetRegistry().on_construct<SpriteComponent>().connect<&OnSpriteComponentAdded>();
    }

    void OnSpriteComponentAdded(Registry&, RawEntity entity)
    {
        Entity spriteEntity = entity;
        SpriteComponent& spriteComponent = spriteEntity.Get<SpriteComponent>();
        spriteComponent.SpriteAssetRef.Retarget();
    }

    void OnPreDrawPrimitives()
    {
        const auto view = World::GetRegistry().view<TransformComponent, SpriteComponent>();

        World::GetRegistry().sort<SpriteComponent>([](const SpriteComponent& a, const SpriteComponent& b)
        {
            return a.OrderInLayer < b.OrderInLayer;
        });

        view.each([&](RawEntity entity, const TransformComponent& transformComponent, const SpriteComponent& sprite) {

            if (!sprite.IsVisible)
                return;

            SpritePrimitive primitive;
            primitive.FlipMode = sprite.FlipMode;
            primitive.TransformMatrix = transformComponent.GetMatrix();
            primitive.TintColor = sprite.TintColor;
            primitive.UVScale = sprite.UVScale;
            primitive.Size = sprite.Size;

            if (sprite.SpriteAssetRef.IsValid())
            {
                auto spritePtr = sprite.SpriteAssetRef.GetWeakAs<Sprite>();
                if (!spritePtr.expired() && 
                    sprite.SpriteAssetRef.GetWeak().lock()->GetAssetData().AssetType == Type::get<Sprite>().get_name())
                {
                    primitive.SpriteRef = spritePtr.lock();
                }
            }

            primitive.Source = sprite.Source;
            primitive.SubSpriteName = sprite.SubSpriteName;
            primitive.EntityID = (int) entity;

            primitive.GenerateVertexData();

            Renderer::SubmitSpritePrimitive(primitive);
        });
    }
}
