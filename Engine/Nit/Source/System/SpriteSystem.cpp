#include "SpriteSystem.h"
#include "Core\Engine.h"
#include "Render\Renderer.h"
#include "Component\SpriteComponent.h"
#include "Component\TransformComponent.h"

namespace Nit::SpriteSystem
{
    const String SystemID = "SpriteSystem";

    void OnCreate();
    void OnSpriteComponentAdded(Registry&, RawEntity entity);
    void OnSpriteComponentRemoved(Registry&, RawEntity entity);
    void OnPreDrawPrimitives();

    void Register()
    {
        Engine::CreateSystem(SystemID, SpriteExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
    }

    void OnCreate()
    {
        World::GetRegistry().on_construct<SpriteComponent>().connect<&OnSpriteComponentAdded>();
        World::GetRegistry().on_destroy<SpriteComponent>().connect<&OnSpriteComponentRemoved>();
    }

    void OnSpriteComponentAdded(Registry&, RawEntity entity)
    {
        Entity spriteEntity = entity;
        SpriteComponent& spriteComponent = spriteEntity.Get<SpriteComponent>();
        spriteComponent.SpriteAssetRef.Retarget();

        spriteComponent.Primitive = Renderer::CreatePrimitive<SpritePrimitive>();
    }

    void OnSpriteComponentRemoved(Registry&, RawEntity entity)
    {
        Entity spriteEntity = entity;
        SpriteComponent& spriteComponent = spriteEntity.Get<SpriteComponent>();
        
        Renderer::DestroyPrimitive(spriteComponent.Primitive);
        spriteComponent.Primitive = nullptr;
    }

    void OnPreDrawPrimitives()
    {
        const auto view = World::GetRegistry().view<TransformComponent, SpriteComponent>();

        view.each([&](RawEntity rawEntity, const TransformComponent& transformComponent, const SpriteComponent& sprite) {

            Entity entity = rawEntity;

            auto& primitive = *sprite.Primitive;

            if (!sprite.IsVisible)
            {
                primitive.bIsVisible = false;
                return;
            }

            if (sprite.SpriteAssetRef.Is<Sprite>())
            {
                Sprite& spriteAsset = sprite.SpriteAssetRef.As<Sprite>();

                if (sprite.bUseSubsprite && spriteAsset.ContainsSubSprite(sprite.SubSpriteName))
                {
                    SubSprite subSprite = spriteAsset.GetSubSprite(sprite.SubSpriteName);
                    primitive.VertexPositions = subSprite.VertexPositions;
                    primitive.VertexUVs = subSprite.VertexUVs;
                }
                else
                {
                    primitive.VertexPositions = spriteAsset.GetVertexPositions();
                    primitive.VertexUVs = spriteAsset.GetVertexUVs();
                }

                primitive.TextureID = spriteAsset.GetRendererId();
            }
            else
            {
                SpritePrimitive defaultSprite;
                sprite.Primitive->VertexPositions = defaultSprite.VertexPositions; 
                sprite.Primitive->VertexUVs       = defaultSprite.VertexUVs;
                sprite.Primitive->TextureID       = 0;
            }

            primitive.bIsVisible = true;
            primitive.bFlipX = sprite.bFlipX;
            primitive.bFlipY = sprite.bFlipY;

            primitive.Transform = transformComponent.GetMatrix();
            primitive.TintColor = sprite.TintColor;
            primitive.UVScale = sprite.UVScale;
            primitive.Size = sprite.Size;
            
            primitive.EntityID = (int) rawEntity;
            primitive.SortingLayer = sprite.SortingLayer;
        });
    }
}
