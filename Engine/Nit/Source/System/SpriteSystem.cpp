#include "SpriteSystem.h"

#include "FamilySystem.h"
#include "Core\Engine.h"
#include "Render\Renderer.h"
#include "Component\SpriteComponent.h"
#include "Component\TransformComponent.h"
#include "Render/RenderComponents.h"

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

        spriteComponent.renderEntity = Renderer::CreateRenderEntity();
        spriteComponent.renderEntity.Add<Primitive2DComponent>();
        spriteComponent.renderEntity.Add<SpriteShapeComponent>();
    }

    void OnSpriteComponentRemoved(Registry&, RawEntity entity)
    {
        Entity spriteEntity = entity;
        SpriteComponent& spriteComponent = spriteEntity.Get<SpriteComponent>();
        
        //Renderer::DestroyPrimitive(spriteComponent.Primitive);
        //spriteComponent.Primitive = nullptr;
    }

    void OnPreDrawPrimitives()
    {
        const auto view = World::GetRegistry().view<TransformComponent, SpriteComponent>();

        view.each([&](RawEntity rawEntity, const TransformComponent& transformComponent, SpriteComponent& sprite) {

            Entity entity = rawEntity;

            auto& primitive2DComponent = sprite.renderEntity.Get<Primitive2DComponent>();
            auto& spriteShapeComponent = sprite.renderEntity.Get<SpriteShapeComponent>();
            
            if (!sprite.IsVisible)
            {
                sprite.renderEntity.SetVisible(false);
                return;
            }

            if (sprite.SpriteAssetRef.Is<Sprite>())
            {
                Sprite& spriteAsset = sprite.SpriteAssetRef.As<Sprite>();

                if (sprite.bUseSubsprite && spriteAsset.ContainsSubSprite(sprite.SubSpriteName))
                {
                    SubSprite subSprite = spriteAsset.GetSubSprite(sprite.SubSpriteName);
                    primitive2DComponent.VertexPositions = subSprite.VertexPositions;
                    primitive2DComponent.VertexUVs = subSprite.VertexUVs;
                }
                else
                {
                    primitive2DComponent.VertexPositions = spriteAsset.GetVertexPositions();
                    primitive2DComponent.VertexUVs = spriteAsset.GetVertexUVs();
                }

                spriteShapeComponent.TextureID = spriteAsset.GetRendererId();
            }
            else
            {
                SpritePrimitive defaultSprite;
                primitive2DComponent.VertexPositions = defaultSprite.VertexPositions; 
                primitive2DComponent.VertexUVs   = defaultSprite.VertexUVs;
                spriteShapeComponent.TextureID       = 0;
            }

            sprite.renderEntity.SetVisible(true);
            spriteShapeComponent.bFlipX = sprite.bFlipX;
            spriteShapeComponent.bFlipY = sprite.bFlipY;

            sprite.renderEntity.SetTransform(FamilySystem::GetMatrix(entity));
            primitive2DComponent.TintColor = sprite.TintColor;
            spriteShapeComponent.UVScale = sprite.UVScale;
            spriteShapeComponent.Size = sprite.Size;

            sprite.renderEntity.Get<RenderComponent>().EntityID = (int) rawEntity;
            primitive2DComponent.SortingLayer = sprite.SortingLayer;
        });
    }
}
