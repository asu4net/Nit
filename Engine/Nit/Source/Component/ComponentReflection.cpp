#include "ComponentReflection.h"

namespace Nit
{
    ComponentType::ComponentType(Type type, const FnHas& hasFunction, const FnGetByCopy& getByCopyFunction,
        const FnAddDefault& addDefaultFunction, const FnAdd& addFunction, const FnRemove& removeFunction, const FnSet& setFunction)
        : m_Type(type)
        , m_HasFunction(hasFunction)
        , m_GetByCopyFunction(getByCopyFunction)
        , m_AddDefaultFunction(addDefaultFunction)
        , m_AddFunction(addFunction)
        , m_RemoveFunction(removeFunction)
        , m_SetFunction(setFunction)
    {
    }

    bool ComponentType::HasComponent(const Entity& actor)
    {
        return m_HasFunction(actor);
    }

    Instance ComponentType::GetComponentByCopy(const Entity& actor)
    {
        return m_GetByCopyFunction(actor);
    }

    void ComponentType::AddDefaultComponent(Entity& actor)
    {
        return m_AddDefaultFunction(actor);
    }

    void ComponentType::AddComponent(Entity& actor, const Instance& instance)
    {
        return m_AddFunction(actor, instance);
    }

    void ComponentType::RemoveComponent(Entity& actor)
    {
        return m_RemoveFunction(actor);
    }

    void ComponentType::SetComponentData(Entity& actor, const Instance& instance)
    {
        m_SetFunction(actor, instance);
    }

    const SharedPtr<ComponentType>& ComponentReflection::GetComponentOfType(Type type)
    {
        return m_Components[type];
    }

    void ComponentReflection::Each(Delegate<void(Type, const SharedPtr<ComponentType>&)> iterateFunc)
    {
        for (auto& [type, componentType] : m_Components)
        {
            iterateFunc(type, componentType);
        }
    }
}