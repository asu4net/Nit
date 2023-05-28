#include "NitPCH.h"
#include "World.h"

#include "Core/Asset/AssetManager.h"

namespace Nit
{
    void World::LoadScene(const std::string& name)
    {
        if (!m_Scenes.contains(name))
            return;
        
        if (m_ActiveScene)
        {
            m_ActiveScene->Finish();
        }

        m_ActiveScene = m_Scenes[name];
        m_ActiveScene->Start();
    }
    
    void World::OnStart()
    {
        GetAllSceneAssets();
        if (m_ActiveScene)
            m_ActiveScene->Start();
    }

    void World::GetAllSceneAssets()
    {
        std::vector<AssetLink> assetLinks;
        AssetManager::GetInstance().GetAssetsOfType<Scene>(assetLinks);
        
        if (assetLinks.empty())
            return;
        
        for(AssetLink& link : assetLinks)
        {
            const Shared<Scene> scene = link.GetAs<Scene>();
            
            if (scene->IsStarterScene())
                m_ActiveScene = scene;
            
            m_Scenes[scene->GetName()] = scene;
            scene->SetWeakPtr(scene);
        }
    }

    void World::OnUpdate(const TimeStep& timeStep)
    {
        if (m_ActiveScene)
            m_ActiveScene->Update(timeStep);
    }
    
    void World::OnFixedUpdate(const TimeStep& timeStep)
    {
        if (m_ActiveScene)
            m_ActiveScene->FixedUpdate(timeStep);
    }

    void World::OnPostFixedUpdate(const TimeStep& timeStep)
    {
        if (m_ActiveScene)
            m_ActiveScene->Draw();
    }

    void World::OnFinish()
    {
        if (m_ActiveScene)
            m_ActiveScene->Finish();
    }
}