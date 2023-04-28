#pragma once

namespace Nit
{
    class Asset
    {
    public:
        Asset();
        Asset(Asset& other) = default;
        Asset(const std::string& name, const std::string& path, Id id);

        const std::string& GetName() const { return m_Name; }
        const std::string& GetPath() const { return m_Path; }
        Id GetId() { return m_Id; }
        
    private:
        std::string m_Name;
        std::string m_Path;
        Id m_Id;
        
        RTTR_REGISTRATION_FRIEND
        RTTR_ENABLE()
    };
}
