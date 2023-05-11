#include "NitPCH.h"
#include "World.h"

namespace Nit
{
    void World::OnStart()
    {
        m_ActiveScene = CreateShared<Scene>();
        m_ActiveScene->SetWeakPtr(m_ActiveScene);
        m_ActiveScene->Start();
    }

    void World::OnUpdate(const TimeStep& timeStep)
    {
        m_ActiveScene->Update(timeStep);
    }

    void World::OnFixedUpdate(const TimeStep& timeStep)
    {
        m_ActiveScene->FixedUpdate(timeStep);
    }

    void World::OnFinish()
    {
        m_ActiveScene->Finish();
    }
}
