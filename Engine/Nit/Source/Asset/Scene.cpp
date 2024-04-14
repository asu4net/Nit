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
    static void SerializeComponent(ComponentType& componentType, const Entity& actor, TStringStream& outStream)
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

    static void DeserializeComponent(const TString& componentTypeStr, const TString& componentDataStr, Entity entity)
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

    TString Scene::GetSceneExstension()
    {
        static const TString s_SceneExtension = ".nit";
        return s_SceneExtension;
    }

    TString Scene::DefaultFolder()
    {
        static const TString s_SceneFolder = "Scenes";
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
        TStringStream sceneStream;
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
        TStringStream jsonSceneStream;

        if (m_Entities.empty())
            return;

        for (Entity& entity : m_Entities)
        {
            if (!entity.Has<IDComponent>()        ||
                !entity.Has<NameComponent>()      ||
                !entity.Has<TransformComponent>() ||
                !entity.Has<SceneComponent>() && entity.Get<SceneComponent>().IsSerializable)
                return;
            
            TString name = entity.GetName().Name;
            name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
            jsonSceneStream << "#entity " << name << " " << std::to_string((uint64_t)entity.GetID().ID) << std::endl;

            ComponentType& transformCmpType = *ComponentReflection::GetComponentOfType(Type::get<TransformComponent>());
            SerializeComponent(transformCmpType, entity, jsonSceneStream);

            ComponentReflection::Each({ [&](Type classType, TSharedPtr<ComponentType> componentType) {

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

        TIStringStream jsonSceneStream{ GetData() };

        Entity entity;
        TString componentDataStr, componentTypeStr;

        bool bComponentCreated = false;

        for (TString line; std::getline(jsonSceneStream, line);)
        {
            if (line.find("#entity") != TString::npos)
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

                TIStringStream lineStream(line);
                for (TString word; std::getline(lineStream, word, ' ');)
                {
                    if (word.find("#entity") != TString::npos) continue; //TODO: Allow spaces in names
                    else if (nameComponent.IsEmpty()) nameComponent = word;
                    else idComponent = std::stoull(word);
                }
                PushEntity(entity);
                World::PushEntity(entity);
            }
            else if (line.find("#component") != TString::npos)
            {
                if (bComponentCreated)
                {
                    DeserializeComponent(componentTypeStr, componentDataStr, entity);
                    componentDataStr.clear();
                    componentTypeStr.clear();
                    bComponentCreated = false;
                }

                TIStringStream lineStream(line);
                for (TString word; std::getline(lineStream, word, ' ');)
                {
                    if (word.find("#component") != TString::npos) continue;
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

    void Scene::PushEntity(Entity entity)
    {
        m_Entities.push_back(entity);
    }

    void Scene::EachEntity(Delegate<void(Entity& e)> callback)
    {
        for (Entity& e : m_Entities)
        {
            callback(e);
        }
    }

    Entity Scene::FindEntity(const TString& name) const
    {
        auto it = std::find_if(m_Entities.begin(), m_Entities.end(),
        [&name](const Entity& entity)
        {
            return entity.GetName() == name;
        });

        return it != m_Entities.end() ? *it : Entity();
    }

    void Scene::PopEntity(Entity entity)
    {
        m_Entities.erase(std::find(m_Entities.begin(), m_Entities.end(), entity));
    }

    void Scene::ClearEntities()
    {
        m_Entities.clear();
    }
}
