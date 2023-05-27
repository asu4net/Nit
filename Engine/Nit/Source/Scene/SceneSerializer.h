#pragma once

namespace Nit
{
    class Scene;
    
    struct ComponentInfo
    {
        std::string ComponentType;
        RTTR_ENABLE()
    };
    
    class SceneSerializer
    {
    public:
        SceneSerializer() = default;
        SceneSerializer(Scene* scene);
        
        void Serialize(std::stringstream& ss);
        void Deserialize(const std::string& data);

    private:
        Scene* m_Scene = nullptr;
    };
}
