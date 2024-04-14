#include "World.h"
#include "Asset/Content.h"
#include "Core/Engine.h"
#include "Component/TransformComponent.h"
#include "Component/SceneComponent.h"
#include "Component/NameComponent.h"
#include "Component/IDComponent.h"

namespace Nit::World
{
    TMap<TString, Scene*> AllScenes;
    TMap<TString, Scene*> OpenedScenes;
    TDynamicArray<Entity> GlobalEntities;
    TMap<Id, Entity> IdEntityMap;
    
    Registry* EntityRegistry = nullptr;
    uint32_t CreatedEntitiesCount = 0;

    static std::pair<Id, RawEntity> CreateEntity(const Id specificId)
    {
        const RawEntity rawEntity = EntityRegistry->create();
        const Id entityId = specificId == Id(0) ? Id() : specificId;
        return { entityId, rawEntity };
    }

    Entity CreateEntity(const EntityCreationParams& params)
    {
        const auto& [id, rawEntity] = CreateEntity(params.SpecificId);
        const TString Name = params.Name.empty() ? TString().append("Entity[" + std::to_string(CreatedEntitiesCount) + "]") : params.Name;
        EntityRegistry->emplace<NameComponent>(rawEntity, Name);
        EntityRegistry->emplace<IDComponent>(rawEntity, id);
        EntityRegistry->emplace<TransformComponent>(rawEntity, params.Position, params.Rotation, params.Scale);
        const Entity entity(rawEntity);
        IdEntityMap[id] = entity; 
        CreatedEntitiesCount++;

        if (!params.SceneOwner.empty())
        {
            if (AllScenes.count(params.SceneOwner))
            {
                EntityRegistry->emplace<SceneComponent>(rawEntity, Content::GetAssetByName(params.SceneOwner), params.IsSerializable);
                AllScenes[params.SceneOwner]->PushEntity(entity);
            }
            else
            {
                NIT_CHECK(false, "The scene should exist!");
            }
        }
        else
        {
            GlobalEntities.push_back(entity);
        }

        return entity;
    }

    void DestroyEntity(Entity entity)
    {
        if (!entity.IsValid())
            return;

        // Remove entity from world
        PopEntity(entity);

        // Remove entity from scene
        if (entity.Has<SceneComponent>())
        {
            auto& sceneComponent = entity.Get<SceneComponent>();
            TSharedPtr<Scene> scene = sceneComponent.OwnerScene.GetAs<Scene>(); 
            OpenedScenes[scene->GetAssetData().Name]->PopEntity(entity);
        }
        // Remove entity from global entities
        else
        {
            auto it = std::find_if(GlobalEntities.begin(), GlobalEntities.end(),
            [&entity] (Entity e)
            {
                return entity == e;
            });
            
            if (it != GlobalEntities.end())
            {
                GlobalEntities.erase(it);
            }
            else
            {
                NIT_CHECK(false, "Trying to remove unexisting entity!");
            }
        }

        // Destroy entity on the registry
        EntityRegistry->destroy(entity.GetRaw());
    }

    void DestroyEntities(Scene& scene)
    {
        scene.EachEntity({
        [](Entity& e)
        {
            PopEntity(e);
            EntityRegistry->destroy(e.GetRaw());
        }});
        scene.ClearEntities();
    }

    void PushEntity(Entity entity)
    {
        if (!entity.IsValid() || !entity.Has<IDComponent>())
        {
            NIT_CHECK(false, "Invalid entity!");
            return;
        }
        IdEntityMap[entity.GetID().ID] = entity;
    }

    void PopEntity(Id entityId)
    {
        if (!IdEntityMap.count(entityId))
        {
            NIT_CHECK(false, "Invalid id!");
            return;
        }
        IdEntityMap.erase(entityId);
    }

    void PopEntity(Entity entity)
    {
        if (!entity.IsValid() || !entity.Has<IDComponent>())
        {
            NIT_CHECK(false, "Invalid entity!");
            return;
        }
        PopEntity(entity.GetID().ID);
    }

    Entity FindEntity(Id id)
    {
        if (!IdEntityMap.count(id))
        {
            return Entity();
        }
        return IdEntityMap[id];
    }

    TDynamicArray<Nit::Entity>& GetGlobalEntities()
    {
        return GlobalEntities;
    }
    
    Entity FindEntityByName(const TString& entityName)
    {
        for (const Entity& entity : GlobalEntities)
        {
             auto it = std::find_if(GlobalEntities.begin(), GlobalEntities.end(),
             [&entityName](const Entity& entity)
             {
                 return entity.GetName() == entityName;
             });
             if (it != GlobalEntities.end())
             {
                 return *it;
             }
        }
        
        for (auto& [name, scene] : OpenedScenes)
        {
            Entity entity = scene->FindEntity(entityName);
            if (entity.IsValid())
            {
                return entity;
            }
        }
        
        return Entity();
    }

    Entity CloneEntity(Entity sourceEntity, const TString& name)
    {
        const RawEntity dstRawEntity = EntityRegistry->create();

        for (auto&& curr : EntityRegistry->storage()) {
            if (auto& storage = curr.second; storage.contains(sourceEntity.GetRaw())) {
                storage.push(dstRawEntity, storage.value(sourceEntity.GetRaw()));
            }
        }

        Entity entity = dstRawEntity;
        entity.GetID().ID = Id();
        entity.GetName().Name = name.empty() ? entity.GetName().Name.append("Clone") : name;

        if (entity.Has<SceneComponent>())
        {
            auto& scene = entity.Get<SceneComponent>();
            scene.OwnerScene.As<Scene>().PushEntity(entity);
        }
        else
        {
            GlobalEntities.push_back(entity);
        }
        
        IdEntityMap[entity.GetID().ID] = entity;
        return entity;
    }

    void EachEntity(Delegate<void(Entity& e)> callback)
    {
        for (auto&[id, e] : IdEntityMap)
        {
            callback(e);
        }
    }

    static void OnSceneCreated(Scene* scene)
    {
        AllScenes[scene->GetAssetData().Name] = scene;
    }

    static void OnSceneDestroyed(Scene* scene)
    {
        DestroyEntities(*scene);
        AllScenes.erase(scene->GetAssetData().Name);
    }

    static bool IsSceneAsset(AssetRef assetRef)
    {
        if (!assetRef.IsValid())
            return false;
        const TSharedPtr<Asset> asset = assetRef.GetWeak().lock();
        return asset->GetAssetData().AssetType == Type::get<Scene>().get_name();
    }

    static Scene* TryConvertToScenePtr(AssetRef assetRef)
    {
        if (!IsSceneAsset(assetRef))
            return nullptr;
        return assetRef.GetAs<Scene>().get();
    }

    void Init()
    {
        EntityRegistry = new Registry();

        Content::OnAssetCreated().Add([](AssetRef assetRef) {
            if (Scene* scene = TryConvertToScenePtr(assetRef))
                OnSceneCreated(scene);
        });

        Content::OnAssetDestroyed().Add([](AssetRef assetRef) {
            if (Scene* scene = TryConvertToScenePtr(assetRef))
                OnSceneDestroyed(scene);
        });
    }

    void Finish()
    {
        GlobalEntities.clear();

        if (EntityRegistry)
        {
            delete EntityRegistry;
            EntityRegistry = nullptr;
        }
    }

    const TMap<TString, Scene*>& GetAllScenes() { return AllScenes; }
    const TMap<TString, Scene*>& GetOpenedScenes() { return OpenedScenes; }

    bool HasRegistry()
    {
        return EntityRegistry;
    }

    Registry* GetRegistryPtr()
    {
        return EntityRegistry;
    }

    Registry& GetRegistry()
    {
        NIT_CHECK(EntityRegistry, "Trying to reference a null EntityRegistry");
        return *EntityRegistry;
    }

    void OpenScene(const TString& sceneName)
    {
        if (!AllScenes.count(sceneName))
        {
            NIT_CHECK(false, "The scene should exist!");
            return;
        }

        if (OpenedScenes.count(sceneName))
        {
            NIT_CHECK(false, "Scene already opened!");
            return;
        }
        
        Scene* scene = AllScenes[sceneName];
        OpenedScenes[sceneName] = scene;
        scene->Deserialize();

        NIT_LOG_TRACE("Scene opened! %s\n", sceneName.c_str());
    }

    void ReloadScene(const TString& sceneName)
    {
        if (!AllScenes.count(sceneName))
        {
            NIT_CHECK(false, "The scene should exist!");
            return;
        }

        if (!OpenedScenes.count(sceneName))
        {
            NIT_CHECK(false, "Scene not opened!");
            return;
        }

        CloseScene(sceneName);
        OpenScene(sceneName);
    }

    void CloseScene(const TString& sceneName)
    {
        if (!AllScenes.count(sceneName))
        {
            NIT_CHECK(false, "The scene should exist!");
            return;
        }

        if (!OpenedScenes.count(sceneName))
        {
            NIT_CHECK(false, "Scene not opened!");
            return;
        }

        Scene* scene = OpenedScenes[sceneName];
        DestroyEntities(*scene);
        OpenedScenes.erase(sceneName);

        NIT_LOG_TRACE("Scene closed! %s\n", sceneName.c_str());
    }

    void SaveScene(const TString& sceneName)
    {
        if (!AllScenes.count(sceneName))
        {
            NIT_CHECK(false, "The scene should exist!");
            return;
        }

        Scene* scene = AllScenes[sceneName];
        scene->Serialize();
        scene->SaveData();
    }

    void OpenDefaultScene()
    {
        if (AllScenes.empty())
            return;
     
        for (auto& [name, scene] : AllScenes)
        {
            if (scene->IsStartScene())
            {
                OpenScene(name);
                break;
            }
        }
    }

    void ResetOpenedScenes()
    {
        for (auto& [name, scene] : OpenedScenes)
        {
            DestroyEntities(*scene);
            scene->Deserialize();
        }
    }

    bool IsSceneOpened(const TString& sceneName)
    {
        return OpenedScenes.count(sceneName);
    }

    void CreateScene(const TString& sceneName)
    {
        if (IsSceneCreated(sceneName))
        {
            NIT_CHECK(IsSceneCreated(sceneName), "Scene already exists!");
            return;
        }

        AssetData data;
        data.Name = sceneName;
        data.Path = Scene::DefaultFolder() + "/" + sceneName + Scene::GetSceneExstension();
        AssetRef sceneRef = Content::CreateAsset<Scene>(data);
        TSharedPtr<Scene> scene = sceneRef.GetWeakAs<Scene>().lock();
        scene->Serialize();
        scene->SaveData();
        Content::TryLoadAsset(sceneRef);
        Content::SerializeAsset(scene, Scene::DefaultFolder());
        OpenScene(sceneName);
    }

    bool IsSceneCreated(const TString& sceneName)
    {
        return AllScenes.count(sceneName);
    }

    void SerializeOpenedScenes()
    {
        for (auto& [name, scene] : OpenedScenes)
        {
            scene->Serialize();
        }
    }
}