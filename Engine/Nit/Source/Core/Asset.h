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
        glm::vec3 m_TestVec = {3, 2, 1};
        glm::vec2 m_TestVec2 = {3, 2};
        glm::vec4 m_TestVec4 = {3, 2, 1, 0};
        Id m_Id;
        
        RTTR_REGISTRATION_FRIEND
        RTTR_ENABLE()
    };
}
