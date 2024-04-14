#pragma once
#include "Entity/Entity.h"

namespace Nit
{
    using FnHas = Delegate<bool(const Entity&)>;
    using FnGetByCopy = Delegate<Instance(const Entity&)>;
    using FnAddDefault = Delegate<void(Entity&)>;
    using FnAdd = Delegate<void(Entity&, const Instance&)>;
    using FnRemove = Delegate<void(Entity&)>;
    using FnSet = Delegate<void(Entity&, const Instance&)>;

    class ComponentType
    {
    public:
        ComponentType(Type type, const FnHas& hasFunction, const FnGetByCopy& getByCopyFunction,
            const FnAddDefault& addDefaultFunction, const FnAdd& addFunction, const FnRemove& removeFunction, const FnSet& setFunction);

        Type GetType() const { return m_Type; }

        bool HasComponent(const Entity& actor);
        Instance GetComponentByCopy(const Entity& actor);
        void AddDefaultComponent(Entity& actor);
        void AddComponent(Entity& actor, const Instance& instance);
        void RemoveComponent(Entity& actor);
        void SetComponentData(Entity& actor, const Instance& instance);

    private:
        Type m_Type;
        FnHas m_HasFunction;
        FnGetByCopy m_GetByCopyFunction;
        FnAddDefault m_AddDefaultFunction;
        FnAdd m_AddFunction;
        FnRemove m_RemoveFunction;
        FnSet m_SetFunction;
    };

    class ComponentReflection
    {
    public:
        template<typename T>
        static void RegisterComponentType()
        {
            Type type = Type::get<T>();
            TSharedPtr<ComponentType> componentType = CreateSharedPtr<ComponentType>(
                type,
                FnHas([](const Entity& actor) -> bool { return actor.Has<T>(); }),
                FnGetByCopy([](const Entity& actor) -> Instance { return actor.Get<T>(); }),
                FnAddDefault([](Entity& actor) -> void { actor.Add<T>(); }),
                FnAdd([](Entity& actor, const Instance& instance) -> void { actor.Add<T>(*instance.try_convert<T>()); }),
                FnRemove([](Entity& actor) -> void { return actor.Remove<T>(); }),
                FnSet([](Entity& actor, const Instance& instance) -> void { actor.Get<T>() = *instance.try_convert<T>(); })
            );
            m_Components[type] = componentType;
        }

        static const TSharedPtr<ComponentType>& GetComponentOfType(Type type);

        static void Each(Delegate<void(Type, const TSharedPtr<ComponentType>&)> iterateFunc);

    private:
        inline static TMap<Type, TSharedPtr<ComponentType>> m_Components;
    };
}