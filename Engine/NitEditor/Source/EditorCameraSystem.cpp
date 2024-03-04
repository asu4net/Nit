#include "EditorCameraSystem.h"
#include "Core/Engine.h"

namespace Nit::EditorCameraSystem
{
    void OnCreate();
    void OnStart();
    void OnUpdate();
    void OnFinish();
    void OnLateUpdate();

    void Register()
    {
        Engine::CreateSystem("EditorCameraSystem", 600, ExecutionContext::Editor);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::LateUpdate, &OnLateUpdate);
    }

    void OnCreate()
    {
    }
    
    void OnStart()
    {
    }
    
    void OnUpdate()
    {
    }
    
    void OnFinish()
    {
    }
    
    void OnLateUpdate()
    {
    }
}