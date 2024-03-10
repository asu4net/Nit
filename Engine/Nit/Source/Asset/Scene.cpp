#include "Scene.h"
#include "Content.h"
#include "Component\TransformComponent.h"
#include "Component\ComponentReflection.h"
#include "Component\SceneComponent.h"
#include "Component\NameComponent.h"
#include "Component\IDComponent.h"
#include "Entity\World.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<Scene>("Scene")
        .property("IsStartScene", &Scene::m_bIsStartScene)
        .constructor<>();
}

namespace Nit
{
    static void SerializeComponent(ComponentType& componentType, const Entity& actor, StringStream& outStream)
    {
        if (!componentType.HasComponent(actor) 
            || componentType.GetType() == Type::get<SceneComponent>()
            || componentType.GetType() == Type::get<NameComponent>()
            || componentType.GetType() == Type::get<IDComponent>())
        {
            return;
        }
        
        outStream << "#component " << componentType.GetType().get_name() << std::endl;
        const Instance componentData = componentType.GetComponentByCopy(actor);
        outStream << Serialization::ToJson(componentData) << std::endl;
    }

    static void DeserializeComponent(const String& componentTypeStr, const String& componentDataStr, Entity entity)
    {
        Type componentClassType = Type::get_by_name(componentTypeStr);
        if (!componentClassType.is_valid())
            return;

        Variant variant = componentClassType.create();
        if (!variant.is_valid())
            return;

        Instance componentInstance = variant;
        if (!componentInstance.is_valid())
            return;

        Serialization::FromJson(componentDataStr, componentInstance);
        auto componentType = ComponentReflection::GetComponentOfType(componentClassType);
        componentType->AddComponent(entity, componentInstance);
    }

    NIT_FORCE_LINK_IMPL(Scene)

    String Scene::GetSceneExstension()
    {
        static const String s_SceneExtension = ".nit";
        return s_SceneExtension;
    }

    String Scene::DefaultFolder()
    {
        static const String s_SceneFolder = "Scenes";
        return s_SceneFolder;
    }

    Scene::Scene() = default;
    Scene::~Scene() = default;

    bool Scene::Load()
    {
        LoadData();
        return true;
    }

    void Scene::LoadData()
    {
        std::ifstream sceneFile(GetAssetData().Path);
        StringStream sceneStream;
        sceneStream << sceneFile.rdbuf();
        m_Data = sceneStream.str();
    }

    void Scene::SaveData()
    {
        std::ofstream sceneFileProject(GetAssetData().Path);
        sceneFileProject << m_Data;
    }

    void Scene::Serialize()
    {
        StringStream jsonSceneStream;

        if (m_Entities.empty())
            return;

        for (Entity& entity : m_Entities)
        {
            if (!entity.Has<IDComponent>()        ||
                !entity.Has<NameComponent>()      ||
                !entity.Has<TransformComponent>() ||
                !entity.Has<SceneComponent>() && entity.Get<SceneComponent>().IsSerializable)
                return;
            
            String name = entity.GetName().Name;
            name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
            jsonSceneStream << "#entity " << name << " " << std::to_string((uint64_t)entity.GetID().ID) << std::endl;

            ComponentType& transformCmpType = *ComponentReflection::GetComponentOfType(Type::get<TransformComponent>());
            SerializeComponent(transformCmpType, entity, jsonSceneStream);

            ComponentReflection::Each({ [&](Type classType, SharedPtr<ComponentType> componentType) {

                if (classType == transformCmpType.GetType())
                    return;
                SerializeComponent(*componentType, entity, jsonSceneStream);
            } });
        }

        m_Data = jsonSceneStream.str();
    }

    void Scene::Deserialize()
    {
        Registry& registry = World::GetRegistry();

        IStringStream jsonSceneStream{ GetData() };

        Entity entity;
        String componentDataStr, componentTypeStr;

        bool bComponentCreated = false;

        for (String line; std::getline(jsonSceneStream, line);)
        {
            if (line.find("#entity") != String::npos)
            {
                if (bComponentCreated)
                {
                    DeserializeComponent(componentTypeStr, componentDataStr, entity);
                    componentDataStr.clear();
                    componentTypeStr.clear();
                    bComponentCreated = false;
                }

                entity = registry.create();
                auto& idComponent = entity.Add<IDComponent>();
                auto& nameComponent = entity.Add<NameComponent>();
                entity.Add<SceneComponent>(Content::GetAssetByName(GetAssetData().Name), true);

                IStringStream lineStream(line);
                for (String word; std::getline(lineStream, word, ' ');)
                {
                    if (word.find("#entity") != String::npos) continue; //TODO: Allow spaces in names
                    else if (nameComponent.IsEmpty()) nameComponent = word;
                    else idComponent = std::stoull(word);
                }
                AddEntity(entity);
            }
            else if (line.find("#component") != String::npos)
            {
                if (bComponentCreated)
                {
                    DeserializeComponent(componentTypeStr, componentDataStr, entity);
                    componentDataStr.clear();
                    componentTypeStr.clear();
                    bComponentCreated = false;
                }

                IStringStream lineStream(line);
                for (String word; std::getline(lineStream, word, ' ');)
                {
                    if (word.find("#component") != String::npos) continue;
                    if (componentTypeStr.empty()) componentTypeStr = word;
                }
                bComponentCreated = true;
            }

            else
            {
                componentDataStr.append(line + "\n");
                continue;
            }
        }
        
        // For the last component
        if (!componentDataStr.empty() && !componentTypeStr.empty())
        {
            DeserializeComponent(componentTypeStr, componentDataStr, entity);
        }
    }

    void Scene::AddEntity(Entity entity)
    {
        m_Entities.push_back(entity);
    }

    void Scene::DestroyEntity(Entity entity)
    {
        auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
        if (it == m_Entities.end())
            return;
        m_Entities.erase(it);
        World::GetRegistry().destroy(entity.GetRaw());
    }

    void Scene::ReleaseEntities()
    {
        // move to world
        for (Entity& entity : m_Entities)
        {
            RawEntity rawEntity = entity.GetRaw();
            World::GetRegistry().destroy(rawEntity);
        }
        m_Entities.clear();
    }
}