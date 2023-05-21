#include "RegistrySerializer.h"
#include "Actor.h"
#include "Scene.h"
#include "Components/DetailsComponent.h"
#include "Core/Serialization.h"

namespace Nit
{
    void RegistrySerializer::Serialize(const Shared<entt::registry>& registry, std::stringstream& ss)
    {
        registry->each([&](entt::entity entity){

            const Actor actor= { entity, registry };
            
            if (!actor.Get<DetailsComponent>().bIsSerializable)
                return;

            for(auto& [type, cmpMetaData] : Scene::ComponentMetaData)
            {
                if (!cmpMetaData.HasFunction(actor)) continue;
                
                rttr::instance instance = cmpMetaData.GetByCopyFunction(actor);
                ss << Serialization::ToJson(instance);
            }
        });
        printf("%s", ss.str().c_str());
    }
}
