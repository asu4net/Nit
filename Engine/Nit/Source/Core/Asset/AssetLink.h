#pragma once
#include "Asset.h"

namespace Nit
{
    class AssetLink
    {
    public:
        AssetLink() = default;
        AssetLink(const std::string& name, const Id& id, const std::string& typeName);
        
        const std::string& GetName() const { return m_Name; }
        Id GetId() const { return m_Id; }
        const std::string& GetTypeName() const { return m_TypeName; }
        
        bool IsValid() const { return Get() != nullptr; }

        Shared<Asset> Get() const;
        
        template<typename T>
        Shared<T> GetAs() const
        {
            const rttr::type t = rttr::type::get<T>();
            if (!t.is_valid() || !t.is_derived_from<Asset>())
                return nullptr;
            return std::static_pointer_cast<T>(Get());
        }
        
    private:
        std::string m_Name{"Uninitialized"};
        Id m_Id{0};
        std::string m_TypeName{"Uninitialized"};

        RTTR_REGISTRATION_FRIEND
        RTTR_ENABLE()
    };
}