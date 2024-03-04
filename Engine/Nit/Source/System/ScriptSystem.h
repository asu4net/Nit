#pragma once

namespace Nit { struct ScriptComponent; }

namespace Nit::ScriptSystem
{
    void Register();
    void OnCreate();
    void OnStart();
    void OnUpdate();
    void OnFixedUpdate();
    void OnPreDrawPrimitives();
    void OnFinish();
    void OnDestroy();

    void OnScriptComponentAdded(Registry&, const RawEntity entity);
    void OnScriptComponentDestroyed(Registry&, const RawEntity entity);
}