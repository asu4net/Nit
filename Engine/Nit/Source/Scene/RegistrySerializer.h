#pragma once

namespace Nit
{
    class RegistrySerializer
    {
    public:
        void Serialize(const Shared<entt::registry>& registry, std::stringstream& ss);
    };
}