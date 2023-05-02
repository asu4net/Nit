#pragma once

namespace Nit
{
    class Asset
    {
    public:
        Asset() = default;
        Asset(const std::string& name, const std::string& path, const Id& id);
        virtual ~Asset() = default;
        
        virtual bool Load() { return false; }
        virtual void Initialize() {}
        virtual bool Unload() { return false; }

        const std::string& GetTypeName() const { return m_TypeName; }
        const std::string& GetName() const { return m_Name; }
        const std::string& GetPath() const { return m_Path; }
        const std::string& GetAbsolutePath() const { return m_AbsolutePath; }
        
        Id GetId() const { return m_Id; }
        
    private:
        std::string m_Name;
        std::string m_Path;
        std::string m_AbsolutePath;
        std::string m_TypeName;
        Id m_Id{0};

        void SetTypeName(const std::string& typeName) { m_TypeName = typeName; }

        friend class AssetManager;
        
        RTTR_REGISTRATION_FRIEND
        RTTR_ENABLE()
    };
}