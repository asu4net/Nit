#pragma once
#include "Core\Engine.h"
#include "Entity\Entity.h"
#include "Entity\World.h"
#include "Script\Script.h"
#include "Asset\Scene.h"

namespace Nit
{
    // INFO: Estos scripts son bastante placeholder
    // TODO: Hacer que los scripts hereden de Asset e integrarlos en ciclo de ejecución de los mismos
    struct ScriptComponent
    {
        String ScriptType;
        uint32_t ExecutionOrder = 0;        
        String PrevScriptType;
        Entity OwnerEntity;
        SharedPtr<Script> ScriptInstance;
        bool bStarted = false;
        
        ScriptComponent() = default;
        
        ScriptComponent(const String& scriptType)
            : ScriptType(scriptType)
        {}

        ScriptComponent(Type type)
            : ScriptType(type.get_name().to_string())
        {}
    };

    NIT_FORCE_LINK(ScriptComponent)
}