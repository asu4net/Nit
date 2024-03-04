#include "Script.h"
#include "Component\IDComponent.h"
#include "Component\NameComponent.h"
#include "Component\TransformComponent.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    
    rttr::registration::class_<Script>("Script")
        .constructor<>();
}

namespace Nit
{
    Script::Script()
        : m_Entity(Entity())
    {}

    Script::~Script() = default;
    
    TransformComponent& Script::GetTransform() const
    {
        return GetEntity().GetTransform();
    }

    IDComponent& Script::GetID() const
    {
        return GetEntity().GetID();
    }

    NameComponent& Script::GetName() const
    {
        return GetEntity().GetName();
    }

    void Script::Create()
    {
        OnCreate();
    }

    void Script::Start()
    {
        OnStart();
    }
    
    void Script::Update()
    {
        OnUpdate();
    }

    void Script::FixedUpdate()
    {
        OnFixedUpdate();
    }

    void Script::SubmitGizmos()
    {
        OnSubmitGizmos();
    }

    void Script::Finish()
    {
        OnFinish();
    }

    void Script::Destroy()
    {
        OnDestroy();
    }

    void Script::OnCreate()
    {
    }

    void Script::OnStart()
    {
    }

    void Script::OnUpdate()
    {
    }

    void Script::OnFinish()
    {
    }

    void Script::OnFixedUpdate()
    {
    }

    void Script::OnSubmitGizmos()
    {
    }

    void Script::OnDestroy()
    {
    }
}