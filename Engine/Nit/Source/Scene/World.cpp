#include "NitPCH.h"
#include "World.h"

#include "Core/Asset/AssetManager.h"

namespace Nit
{
    void World::OnStart()
    {
        AssetManager& assetManager = AssetManager::GetInstance();
        m_ActiveScene = assetManager.GetAssetByName("DefaultScene").GetAs<Scene>();
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

    void World::OnPostFixedUpdate(const TimeStep& timeStep)
    {
        m_ActiveScene->Draw();
    }

    void World::OnFinish()
    {
        m_ActiveScene->Finish();
    }
}
