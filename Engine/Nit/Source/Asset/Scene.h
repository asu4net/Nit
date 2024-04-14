#pragma once
#include "Asset.h"
#include "Entity/Entity.h"

namespace Nit
{
    class ComponentType;
    using EntityArray = TDynamicArray<Entity>;
    
    class Scene : public Asset
    {
    public:
        static TString GetSceneExstension();
        static TString DefaultFolder();

        Scene  ();
        ~Scene ();

        bool        Load          () override;
        void        LoadData      ();
        void        SaveData      ();
        void        Serialize     ();
        void        Deserialize   ();
        bool        IsStartScene  ()                                   const { return m_bIsStartScene; }
        TString      GetData       ()                                   const { return m_Data; }
        void        PushEntity    (Entity entity);
        EntityArray GetEntities   ()                                   const { return m_Entities; }
        void        EachEntity    (Delegate<void(Entity& e)> callback);
        Entity      FindEntity    (const TString& name)                 const;
        void        PopEntity     (Entity entity);
        void        ClearEntities ();
    
    private:
        TString               m_Data;
        bool                 m_bIsStartScene = false;
        TDynamicArray<Entity> m_Entities;
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    NIT_FORCE_LINK(Scene)
}