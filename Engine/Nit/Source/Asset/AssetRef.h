#pragma once
#include "Asset.h"

namespace Nit
{
    class AssetRef
    {
    public:
        AssetRef();
        AssetRef(Id assetId);
        AssetRef(const AssetRef& other);
        AssetRef(AssetRef&& other) noexcept;
        ~AssetRef();

        AssetRef& operator = (const AssetRef& other);
        AssetRef& operator = (AssetRef&& other) noexcept;
        bool operator == (const AssetRef& other) const;
        bool operator != (const AssetRef& other) const;
        
        Id GetAssetId() const { return m_AssetId; }
        bool IsValid() const { return !m_Asset.expired(); }
        WeakPtr<Asset> GetWeak() const { return m_Asset; }
        SharedPtr<Asset> Get() const { return m_Asset.lock(); }

        template<typename T>
        WeakPtr<T> GetWeakAs() const
        { 
            WeakPtr<Asset> asset = GetWeak();
            return IsValid() ? std::static_pointer_cast<T>(Get()) : nullptr;
        }

        template<typename T>
        SharedPtr<T> GetAs() const
        {
            return IsValid() ? std::static_pointer_cast<T>(Get()) : nullptr;
        }

        template<typename T>
        bool Is() const
        {
            return IsValid() ? GetAs<T>().get() : false;
        }

        template<typename T>
        T& As() const
        {
            return *GetAs<T>().get();
        }
        
        void Retarget();
        void Retarget(Id id);
        void Clear();
        
        RTTR_ENABLE_NO_VIRTUAL
        RTTR_REGISTRATION_FRIEND

    private:
        Id m_AssetId;
        WeakPtr<Asset> m_Asset;
    };
}