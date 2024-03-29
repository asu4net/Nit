#pragma once
#include "Asset/AssetRef.h"
#include "Asset/Scene.h"

namespace Nit
{
    class ComponentType;

    struct EntityCreationParams
    {
        String Name;
        String SceneOwner;
        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale = Vector3::One;
        Id SpecificId = 0;
        bool IsSerializable = true;
    };
}

namespace Nit::World
{
    Entity CreateEntity(const EntityCreationParams& params = {});
    void DestroyEntity(Entity entity);

    void PushEntity(Entity entity);
    void PopEntity(Id entityId);
    void PopEntity(Entity entity);
    Entity FindEntity(Id id);

    DynamicArray<Entity>& GetGlobalEntities();

    Entity FindEntityByName(const String& entityName);
    Entity CloneEntity(Entity sourceEntity, const String& name = "");

    //TODO: Destroy entity
    //TODO: Find entity

    void Init();
    void Finish();

    const Map<String, Scene*>& GetAllScenes();
    const Map<String, Scene*>& GetOpenedScenes();

    bool HasRegistry();
    Registry* GetRegistryPtr();
    Registry& GetRegistry();

    void CreateScene(const String& sceneName);
    bool IsSceneOpened(const String& sceneName);
    bool IsSceneCreated(const String& sceneName);
    void OpenScene(const String& sceneName);
    void ReloadScene(const String& sceneName);
    void CloseScene(const String& sceneName);
    void SaveScene(const String& sceneName);
    
    void OpenDefaultScene();
    void SerializeOpenedScenes();
    void ResetOpenedScenes();
}