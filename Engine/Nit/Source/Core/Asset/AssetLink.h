#pragma once
#include "Asset.h"

//TODO: Make parent class to serialize some data

namespace Nit
{
    template<typename T>
    class AssetLink
    {
    public:
        const std::string& GetName() const { return m_Name; }
        Id GetId() const { return m_Id; }
        bool IsValid() const { return m_TargetAsset.expired(); }

        Shared<T> Lock()
        {
            return m_TargetAsset.lock();
        }
        
        void SetTarget(const Weak<T>& asset)
        {
            m_TargetAsset = asset;
            m_Name = Lock()->GetName();
            m_Id = Lock()->GetId();
        }
        
    private:
        std::string m_Name{"Uninitialized"};
        Id m_Id{0};
        Weak<T> m_TargetAsset;
    };
}