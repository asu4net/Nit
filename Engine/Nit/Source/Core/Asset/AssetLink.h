#pragma once
#include "Asset.h"

//TODO: Make parent class to serialize some data

namespace Nit
{
    template<typename T>
    class AssetLink
    {
    public:
        AssetLink()
            : m_Name("Uninitialized")
            , m_Id(0)
            , m_bInitialized(false)
        {
        }

        AssetLink(const std::string& name, const Id& id)
            : m_Name(name)
            , m_Id(id)
            , m_bInitialized(false)
        {
        }
        
        const std::string& GetName() const { return m_Name; }
        Id GetId() const { return m_Id; }
        bool IsInitialized() const { return m_bInitialized; }
        bool IsValid() const { return m_AssetRef.expired(); }

        void Initialize(const Weak<T>& asset)
        {
            if (!IsValid())
                return;
            const rttr::type t = rttr::type::get<T>();
            if (!t.is_valid() || !t.is_derived_from<Asset>())
                return;
            m_AssetRef = asset;
            m_bInitialized = true;
        }
        
        Shared<T> Lock()
        {
            return m_AssetRef.lock();
        }
        
    private:
        std::string m_Name;
        Id m_Id;
        Weak<T> m_AssetRef;
        bool m_bInitialized;
    };
}