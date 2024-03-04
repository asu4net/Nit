#include "ScriptSystem.h"
#include "Asset/Asset.h"
#include "Component/ScriptComponent.h"

namespace Nit::ScriptSystem
{
    const String SystemID = "ScriptSystem";

    void Register()
    {
        Engine::CreateSystem(SystemID, ScriptExecutionOrder, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::FixedUpdate, &OnFixedUpdate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
        Engine::SetSystemCallback(SystemStage::Finish, &OnFinish);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);
    }

    void UpdateScriptInstance(ScriptComponent& scriptComponent)
    {
        NIT_CHECK(scriptComponent.OwnerEntity.IsValid(), "Owner actor should be initialized!");

        // If the script type has not changed do nothing
        if (scriptComponent.ScriptType == scriptComponent.PrevScriptType)
        {
            return;
        }

        // Otherwise if there was a previous instance dispose it
        if (scriptComponent.ScriptInstance)
        {
            scriptComponent.ScriptInstance->Finish();
            scriptComponent.ScriptInstance->Destroy();
            scriptComponent.ScriptInstance = nullptr;
        }
        
        // Clean the previous script type
        scriptComponent.PrevScriptType = "";

        Type scriptType = Type::get_by_name(scriptComponent.ScriptType);

        // If the new script type is not valid clean and do nothing
        if (!scriptType.is_valid() || !scriptType.is_derived_from(Type::get<Script>()))
        {
            scriptComponent.ScriptType = "";
            return;
        }
        
        // Otherwise create an instance of the new script type
        SharedPtr<Script> scriptInstance = scriptType.create().get_value<SharedPtr<Script>>();
        scriptInstance->SetEntity(scriptComponent.OwnerEntity);
        scriptInstance->Create();

        scriptComponent.ScriptInstance = scriptInstance;

        // Update the previous script type
        scriptComponent.PrevScriptType = scriptComponent.ScriptType;
        scriptComponent.bStarted = false;
    }

    void OnScriptComponentAdded(Registry&, const RawEntity rawEntity)
    {
        Entity scriptEntity = rawEntity ;
        ScriptComponent& scriptComponent = scriptEntity.Get<ScriptComponent>();
        scriptComponent.OwnerEntity = scriptEntity;

        World::GetRegistry().sort<ScriptComponent>([](const ScriptComponent& a, const ScriptComponent& b)
        {
            return a.ExecutionOrder < b.ExecutionOrder;
        });

        UpdateScriptInstance(scriptComponent);
    }
    
    void OnScriptComponentDestroyed(Registry&, const RawEntity entity)
    {
        Entity scriptEntity = entity;
        ScriptComponent& scriptComponent = scriptEntity.Get<ScriptComponent>();
        if (!scriptComponent.ScriptInstance) return;

        scriptComponent.ScriptInstance->Finish();
        scriptComponent.ScriptInstance->Destroy();
    }

    void OnCreate()
    {
        World::GetRegistry().on_construct<ScriptComponent>().connect<&OnScriptComponentAdded>();
        World::GetRegistry().on_destroy<ScriptComponent>().connect<&OnScriptComponentAdded>();
    }

    void OnStart()
    {
        auto view = World::GetRegistry().view<ScriptComponent>();
        view.each([&](RawEntity entity, ScriptComponent& script) {
            if (!script.ScriptInstance) return;
            if (!script.bStarted)
            {
                script.ScriptInstance->Start();
                script.bStarted = true;
            }
        });
    }

    void OnUpdate()
    {
        auto view = World::GetRegistry().view<ScriptComponent>();
        view.each([&](RawEntity entity, ScriptComponent& script) {
            UpdateScriptInstance(script);
            if (!script.ScriptInstance) return;
            script.ScriptInstance->Update();
        });
    }

    void OnFixedUpdate()
    {
        auto view = World::GetRegistry().view<ScriptComponent>();
        view.each([&](RawEntity entity, const ScriptComponent& script) {
            if (!script.ScriptInstance) return;
            script.ScriptInstance->FixedUpdate();
        });
    }

    void OnPreDrawPrimitives()
    {
        auto view = World::GetRegistry().view<ScriptComponent>();
        view.each([&](RawEntity entity, const ScriptComponent& script) {
            if (!script.ScriptInstance) return;
            script.ScriptInstance->SubmitGizmos();
        });
    }

    void OnFinish()
    {
        auto view = World::GetRegistry().view<ScriptComponent>();
        view.each([&](RawEntity entity, ScriptComponent& script) {
            
            if (!script.ScriptInstance) return;

            if (script.bStarted)
            {
                script.ScriptInstance->Finish();
                script.bStarted = false;
            }
        });
    }

    void OnDestroy()
    {
        auto view = World::GetRegistry().view<ScriptComponent>();
        view.each([&](RawEntity entity, ScriptComponent& script) {
            if (!script.ScriptInstance) return;
            script.ScriptInstance->Destroy();
        });
    }
}