#include "Scene.h"

#include "Components/DetailsComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/CameraSystem.h"
#include "Systems/SceneSystem.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<Scene>("Scene")
        .constructor<>()
        .constructor<const std::string&, const std::string&, const Id&>();
}

namespace Nit
{
    Scene::Scene(const std::string& name, const std::string& path, const Id& id)
        : Asset(name, path, id)
        , m_SceneSerializer(this)
    {
    }

    bool Scene::Load()
    {
        const std::ifstream stream(GetAbsolutePath());
        
        if (stream.fail())
            return false;

        std::stringstream ss;
        ss << stream.rdbuf();
        m_SceneData = ss.str();
        
        return true;
    }
    
    void Scene::Initialize() {}
    
    bool Scene::Unload()
    {
        std::ofstream fileOut(GetAbsolutePath());
        fileOut << m_SceneData;
        return true;
    }

    void Scene::Start()
    {
        m_Registry = CreateShared<entt::registry>();
        
        rttr::array_range<rttr::type> derivedSystems = rttr::type::get<SceneSystem>().get_derived_classes();
        m_Systems.reserve(derivedSystems.size());
        for (auto& systemType : derivedSystems)
        {
            if (!systemType.is_valid())
                continue;
            
            rttr::variant systemInstance = systemType.create({m_WeakPtr});

            if (!systemInstance.is_valid())
                continue;

            m_Systems.push_back(systemInstance.get_value<Shared<SceneSystem>>());
        }

        std::ranges::sort(m_Systems,
            [&](const Shared<SceneSystem>& a, const Shared<SceneSystem>& b){
                return a->GetExecutionOrder() < b->GetExecutionOrder();
            });
        
        for (const auto& system : m_Systems)
        {
            system->OnStart();
        }

        m_SceneSerializer.Deserialize(m_SceneData);
    }

    void Scene::Update(const TimeStep& timeStep)
    {
        for (const auto& system : m_Systems)
        {
            system->OnUpdate(timeStep);
        }
    }

    void Scene::FixedUpdate(const TimeStep& timeStep)
    {
        for (const auto& system : m_Systems)
        {
            system->OnFixedUpdate(timeStep);
        }
    }

    void Scene::Draw()
    {
        m_SceneRenderer.DrawScene();
    }

    void Scene::Finish()
    {
        for (const auto& system : m_Systems)
        {
            system->OnFinish();
        }
        m_Registry->clear();
        m_Registry.reset();
    }

    void Scene::Save()
    {
        std::stringstream ss;
        m_SceneSerializer.Serialize(ss);
        m_SceneData = ss.str();
    }

    Actor Scene::CreateActorWithId(Id id, const std::string& name, const Vec3& position, const Quat& rotation, const Vec3& scale)
    {
        const entt::entity entity = m_Registry->create();
        m_IdEntityMap[id] = entity;
        Actor actor{entity, m_Registry };
        const bool bIsSerializable = !m_bRuntimeEnabled;
        actor.Add<DetailsComponent>(name, "Default", id, bIsSerializable);
        actor.Add<TransformComponent>(position, rotation, scale);
        return actor;
    }
    
    Actor Scene::CreateActor(const std::string& name, const Vec3& position, const Quat& rotation, const Vec3& scale)
    {
        return CreateActorWithId(Id(), name, position, rotation, scale);
    }

    void Scene::DestroyActor(const Actor& actor)
    {
        if (!actor.IsValid()) return;
        m_IdEntityMap.erase(actor.Get<DetailsComponent>().GetId());
        m_Registry->destroy(actor.GetEntity());
    }
}