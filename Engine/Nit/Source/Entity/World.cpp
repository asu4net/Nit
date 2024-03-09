#include "World.h"
#include "Asset/Content.h"
#include "Core/Engine.h"
#include "Component/TransformComponent.h"
#include "Component/SceneComponent.h"
#include "Component/NameComponent.h"
#include "Component/IDComponent.h"

namespace Nit::World
{
    Map<String, Scene*> AllScenes;
    Map<String, Scene*> OpenedScenes;
    DynamicArray<Entity> GlobalEntities;

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
        const String Name = params.Name.empty() ? String().append("Entity[" + std::to_string(CreatedEntitiesCount) + "]") : params.Name;
        EntityRegistry->emplace<NameComponent>(rawEntity, Name);
        EntityRegistry->emplace<IDComponent>(rawEntity, id);
        EntityRegistry->emplace<TransformComponent>(rawEntity, params.Position, params.Rotation, params.Scale);
        const Entity entity(rawEntity);
        CreatedEntitiesCount++;

        if (!params.SceneOwner.empty())
        {
            if (AllScenes.count(params.SceneOwner))
            {
                EntityRegistry->emplace<SceneComponent>(rawEntity, Content::GetAssetByName(params.SceneOwner), params.IsSerializable);
                AllScenes[params.SceneOwner]->AddEntity(entity);
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

        if (entity.Has<SceneComponent>())
        {
            auto& scene = entity.Get<SceneComponent>();
            OpenedScenes[scene.OwnerScene.GetPtr().lock()->GetAssetData().Name]->DestroyEntity(entity);
            return;
        }

        auto it = std::find(GlobalEntities.begin(), GlobalEntities.end(), entity);
        if (it == GlobalEntities.end())
        {
            return;
        }

        GlobalEntities.erase(it);
        World::GetRegistry().destroy(entity.GetRaw());
    }

    void PushGlobalEntity(Entity entity)
    {
        if (!entity.IsValid() || entity.Has<SceneComponent>())
        {
            NIT_CHECK(false, "Invalid entity!");
            return;
        }
        GlobalEntities.push_back(entity);
    }

    void PopGlobalEntity(Entity entity)
    {
        auto it = std::find_if(GlobalEntities.begin(), GlobalEntities.end(), [&entity] (Entity e) { return entity == e; });
        if (it == GlobalEntities.end())
        {
            GlobalEntities.erase(it);
            return;
        }
        NIT_CHECK(false, "Trying to remove unexisting entity!");
    }

    DynamicArray<Nit::Entity>& GetGlobalEntities()
    {
        return GlobalEntities;
    }

    static void OnSceneCreated(Scene* scene)
    {
        AllScenes[scene->GetAssetData().Name] = scene;
    }

    static void OnSceneDestroyed(Scene* scene)
    {
        scene->ReleaseEntities();
        AllScenes.erase(scene->GetAssetData().Name);
    }

    static bool IsSceneAsset(AssetRef assetRef)
    {
        if (!assetRef.IsValid())
            return false;
        const SharedPtr<Asset> asset = assetRef.GetPtr().lock();
        return asset->GetAssetData().AssetType == Type::get<Scene>().get_name();
    }

    static Scene* TryConvertToScenePtr(AssetRef assetRef)
    {
        if (!IsSceneAsset(assetRef))
            return nullptr;
        return assetRef.GetPtrAs<Scene>().lock().get();
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

    const Map<String, Scene*>& GetAllScenes() { return AllScenes; }
    const Map<String, Scene*>& GetOpenedScenes() { return OpenedScenes; }

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

    void OpenScene(const String& sceneName)
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

    void ReloadScene(const String& sceneName)
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

    void CloseScene(const String& sceneName)
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
        scene->ReleaseEntities();
        OpenedScenes.erase(sceneName);

        NIT_LOG_TRACE("Scene closed! %s\n", sceneName.c_str());
    }

    void SaveScene(const String& sceneName)
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
            scene->ReleaseEntities();
            scene->Deserialize();
        }
    }

    bool IsSceneOpened(const String& sceneName)
    {
        return OpenedScenes.count(sceneName);
    }

    void CreateScene(const String& sceneName)
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
        SharedPtr<Scene> scene = sceneRef.GetPtrAs<Scene>().lock();
        scene->Serialize();
        scene->SaveData();
        Content::TryLoadAsset(sceneRef);
        Content::SerializeAsset(scene, Scene::DefaultFolder());
        OpenScene(sceneName);
    }

    bool IsSceneCreated(const String& sceneName)
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