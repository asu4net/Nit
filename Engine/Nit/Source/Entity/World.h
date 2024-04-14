#pragma once
#include "Asset/AssetRef.h"
#include "Asset/Scene.h"

namespace Nit
{
    class ComponentType;

    struct EntityCreationParams
    {
        TString Name;
        TString SceneOwner;
        CVector3 Position;
        CVector3 Rotation;
        CVector3 Scale = CVector3::One;
        Id SpecificId = 0;
        bool IsSerializable = true;
    };
}

namespace Nit::World
{
    Entity                CreateEntity      (const EntityCreationParams& params = {});
    void                  DestroyEntity     (Entity entity);
    void                  PushEntity        (Entity entity);
    void                  PopEntity         (Id entityId);
    void                  PopEntity         (Entity entity);
    Entity                FindEntity        (Id id);
    TDynamicArray<Entity>& GetGlobalEntities ();
    Entity                FindEntityByName  (const TString& entityName);
    Entity                CloneEntity       (Entity sourceEntity, const TString& name = "");
    void                  EachEntity        (Delegate<void(Entity& e)> callback);
    
    void Init();
    void Finish();

    const TMap<TString, Scene*>& GetAllScenes();
    const TMap<TString, Scene*>& GetOpenedScenes();

    bool HasRegistry();
    Registry* GetRegistryPtr();
    Registry& GetRegistry();

    void CreateScene(const TString& sceneName);
    bool IsSceneOpened(const TString& sceneName);
    bool IsSceneCreated(const TString& sceneName);
    void OpenScene(const TString& sceneName);
    void ReloadScene(const TString& sceneName);
    void CloseScene(const TString& sceneName);
    void SaveScene(const TString& sceneName);
    
    void OpenDefaultScene();
    void SerializeOpenedScenes();
    void ResetOpenedScenes();
}