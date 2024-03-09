#pragma once
#include "Asset.h"
#include "Entity/Entity.h"

namespace Nit
{
    class ComponentType;

    class Scene : public Asset
    {
    public:
        static String GetSceneExstension();
        static String DefaultFolder();

        Scene();
        ~Scene();

        bool Load() override;
        
        void LoadData();
        void SaveData();

        void Serialize();
        void Deserialize();

        String GetData() const { return m_Data; }

        void AddEntity(Entity entity);
        void DestroyEntity(Entity entity);

        const DynamicArray<Entity>& GetEntities() const { return m_Entities; }
        void ReleaseEntities();

    private:
        String m_Data;
        DynamicArray<Entity> m_Entities;
        
        RTTR_ENABLE(Asset)
    };

    NIT_FORCE_LINK(Scene)
    YDECLARE_ASSET_EXTENSION(Scene, nit)
}