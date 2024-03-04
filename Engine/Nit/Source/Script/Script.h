#pragma once
#include "Asset\Scene.h"

namespace Nit
{
    struct TransformComponent;
    struct IDComponent;
    struct NameComponent;

    class Script
    {
    public:
        Script();
        virtual ~Script();

        void SetEntity(Entity entity) { m_Entity = entity; }
        Entity GetEntity() const { return m_Entity; }
        
        TransformComponent& GetTransform() const;
        IDComponent& GetID() const;
        NameComponent& GetName() const;

        template<typename T>
        bool Has() const
        {
            return m_Entity.Has<T>();
        }

        template<typename T, typename... TArgs>
        T& Add(TArgs&&... args) const
        {
            return m_Entity.Add<T>(std::forward<TArgs>(args)...);
        }

        template<typename T>
        T& Get() const
        {
            return m_Entity.Get<T>();
        }

        template<typename T>
        void Remove() const
        {
            m_Entity.Remove<T>();
        }

        void Create();
        void Start();
        void Update();
        void FixedUpdate();
        void SubmitGizmos();
        void Finish();
        void Destroy();

    protected:
        virtual void OnCreate();
        virtual void OnStart();
        virtual void OnUpdate();
        virtual void OnFinish();
        virtual void OnFixedUpdate();
        virtual void OnSubmitGizmos();
        virtual void OnDestroy();

    private:
        Entity m_Entity;

        RTTR_ENABLE()
    };
}