#include "MaterialSystem.h"
#include "Core\Engine.h"
#include "Component\MaterialComponent.h"
#include "Component\SpriteComponent.h"
#include "Render\Renderer.h"
#include "Component\CircleComponent.h"
#include "Asset\Material.h"
#include "Asset\Shader.h"

namespace Nit::MaterialSystem
{
    void OnCreate();
    void OnMaterialComponentAdded(Registry&, RawEntity rawEntity);
    void OnMaterialComponentRemoved(Registry&, RawEntity rawEntity);
    void OnPostDrawPrimitives();

    void Register()
    {
        Engine::CreateSystem("MaterialSystem", MaterialExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::PostDrawPrimitives, &OnPostDrawPrimitives);
    }
    
    void OnCreate()
    {
        World::GetRegistry().on_construct<MaterialComponent>().connect<&OnMaterialComponentAdded>();
        World::GetRegistry().on_destroy<MaterialComponent>().connect<&OnMaterialComponentRemoved>();
    }

    void OnMaterialComponentAdded(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        entity.Get<MaterialComponent>().MaterialRef.Retarget();
    }

    void OnMaterialComponentRemoved(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        
        Primitive2D* primitive = nullptr;

        if (entity.Has<SpriteComponent>())
        {
            auto& cmp = entity.Get<SpriteComponent>();
            primitive = cmp.Primitive;
        }
        else if (entity.Has<CircleComponent>())
        {
            auto& cmp = entity.Get<CircleComponent>();
            primitive = cmp.Primitive;
        }

        if (primitive)
        {
            primitive->ShaderID = 0;
        }
    }

    void OnPostDrawPrimitives()
    {
        const auto view = World::GetRegistry().view<MaterialComponent>();

        view.each([&](RawEntity rawEntity, const MaterialComponent& matCmp) {
            
            Entity entity = rawEntity;
            Primitive2D* primitive = nullptr;

            if (entity.Has<SpriteComponent>())
            {
                auto& cmp = entity.Get<SpriteComponent>();
                primitive = cmp.Primitive;
            }
            else if (entity.Has<CircleComponent>())
            {
                auto& cmp = entity.Get<CircleComponent>();
                primitive = cmp.Primitive;
            }

            if (primitive)
            {
                if (matCmp.MaterialRef.IsValid())
                {
                    Material& material = matCmp.MaterialRef.As<Material>();

                    AssetRef shaderRef = material.GetShaderRef();

                    if (!shaderRef.IsValid())
                    {
                        shaderRef.Retarget();
                    }

                    if (shaderRef.IsValid())
                    {
                        Shader& shader = shaderRef.As<Shader>();
                        primitive->ShaderID = shader.GetRendererId();
                    }
                }
            }
        });
    }
}