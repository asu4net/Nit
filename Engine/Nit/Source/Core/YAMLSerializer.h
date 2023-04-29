#pragma once

namespace Nit
{
    class YAMLSerializer
    {
    public:
        static void SerializeObject(
            const rttr::instance& object,
            const std::string& name,
            std::string& result);
    };
}
