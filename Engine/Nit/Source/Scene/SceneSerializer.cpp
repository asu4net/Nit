#include "SceneSerializer.h"
#include "Actor.h"
#include "Scene.h"
#include "Components/DetailsComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Serialization.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    registration::class_<ComponentInfo>("ComponentInfo")
        .constructor<>()
        .property("ComponentType", &ComponentInfo::ComponentType);
}

namespace Nit
{
    SceneSerializer::SceneSerializer(Scene* scene)
        : m_Scene(scene)
    {
    }

    void SceneSerializer::SerializeComponent(const Actor& actor, std::stringstream& ss, const rttr::type& type,
        ComponentMetaData cmpMetaData)
    {
        if (!cmpMetaData.HasFunction(actor)) return;

        ComponentInfo info;
        info.ComponentType = type.get_name().to_string();

        const rttr::instance instance = cmpMetaData.GetByCopyFunction(actor);
        
        ss << "#component info start" << std::endl;
        ss << Serialization::ToJson(info) << std::endl;
        ss << "#component info end" << std::endl;
                
        ss << "#component data start" << std::endl;
        ss << Serialization::ToJson(instance) << std::endl;
        ss << "#component data end" << std::endl;
    }
    
    void SceneSerializer::Serialize(std::stringstream& ss)
    {
        using namespace entt;
        if (!m_Scene) return;
           
        const Shared<registry> registry = m_Scene->GetRegistry().lock();
        
        registry->each([&](const entity entity){

            const Actor actor= { entity, registry };
            
            if (!actor.Has<DetailsComponent>() ||
                !actor.Has<TransformComponent>() ||
                !actor.Get<DetailsComponent>().bIsSerializable)
                    return;
            
            ss << "#actor start" << std::endl;

            const rttr::type detailsCmpType = rttr::type::get<DetailsComponent>();
            SerializeComponent(actor, ss, detailsCmpType, Scene::ComponentMetaData[detailsCmpType]);

            const rttr::type transformCmpType = rttr::type::get<TransformComponent>();
            SerializeComponent(actor, ss, transformCmpType, Scene::ComponentMetaData[transformCmpType]);
            
            for(auto& [type, cmpMetaData] : Scene::ComponentMetaData)
            {
                if (type == detailsCmpType || type == transformCmpType)
                    continue;
                
                SerializeComponent(actor, ss, type, cmpMetaData);
            }

            ss << "#actor end" << std::endl;
        });
    }

    void SceneSerializer::Deserialize(const std::string& data)
    {
        using namespace entt;
        if (!m_Scene || data.empty()) return;

        const Shared<registry> registry = m_Scene->GetRegistry().lock();
        
        std::istringstream iss{data};
        
        Actor actor;
        std::string componentInfoStr;
        std::string componentDataStr;
        ComponentInfo componentInfo;

        enum class ReadMode
        {
            None,
            ActorStart,
            ActorEnd,
            ComponentInfoStart,
            ComponentInfoEnd,
            ComponentDataStart,
            ComponentDataEnd
        };
        
        ReadMode readMode = ReadMode::None;
        
        for (std::string line; std::getline(iss, line); )
        {
            if (line.find("#actor start") != std::string::npos)
            {
                readMode = ReadMode::ActorStart;
            }

            if (line.find("#actor end") != std::string::npos)
            {
                readMode = ReadMode::ActorEnd;
            }
            
            if (line.find("#component info start") != std::string::npos)
            {
                readMode = ReadMode::ComponentInfoStart;
                continue;
            }

            if (line.find("#component info end") != std::string::npos)
            {
                readMode = ReadMode::ComponentInfoEnd;
            }

            if (line.find("#component data start") != std::string::npos)
            {
                readMode = ReadMode::ComponentDataStart;
                continue;
            }

            if (line.find("#component data end") != std::string::npos)
            {
                readMode = ReadMode::ComponentDataEnd;
            }

            // Serialization

            switch (readMode)
            {
            case ReadMode::ActorStart:
                {
                    actor = { registry->create(), registry };
                }
                continue;
            case ReadMode::ComponentInfoStart:
                {
                    componentInfoStr.append(line + "\n");
                }
                continue;
            case ReadMode::ComponentInfoEnd:
                {
                    Serialization::FromJson(componentInfoStr, componentInfo);
                    componentInfoStr.clear();
                }
                continue;
            case ReadMode::ComponentDataStart:
                {
                    componentDataStr.append(line + "\n");
                }
                continue;
            case ReadMode::ComponentDataEnd:
                {
                    rttr::type componentType = rttr::type::get_by_name(componentInfo.ComponentType);
                    if (!componentType.is_valid())
                        continue;

                    rttr::variant variant = componentType.create();
                    if (!variant.is_valid())
                        continue; 
                    
                    rttr::instance componentInstance = variant;
                    if (!componentInstance.is_valid())
                        continue;
                    
                    Serialization::FromJson(componentDataStr, componentInstance);
                    auto& componentMetaData = Scene::ComponentMetaData[componentType];
                    componentMetaData.AddFunction(actor, componentInstance);
                    componentDataStr.clear();
                }
                continue;
            case ReadMode::ActorEnd:
                m_Scene->m_IdEntityMap[actor.Get<DetailsComponent>().GetId()] = actor.GetEntity();
                continue;
            }
        }
    }
}
