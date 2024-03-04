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
        WeakPtr<Asset> GetPtr() const { return m_Asset; }

        template<typename T>
        WeakPtr<T> GetPtrAs() const
        { 
            WeakPtr<Asset> asset = GetPtr();
            return IsValid() ? std::static_pointer_cast<T>(asset.lock()) : nullptr;
        }

        template<typename T>
        T& GetAs() const
        {
            WeakPtr<T> asset = GetPtrAs<T>();
            NIT_CHECK(IsValid(), "AssetRef needs to point to a valid target!");
            return *asset.lock().get();
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