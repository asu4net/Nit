#pragma once
#include <yaml-cpp/yaml.h>

#include "Asset.h"

namespace Nit
{
    class YAMLSerializer
    {
    public:
        template<typename T>
        YAML::Node Serialize(const T& object)
        {
            YAML::Node node;
            const rttr::type t = object.get_type();
            for (const auto& property : t.get_properties())
            {
                rttr::variant variant = property.get_value(object);
                if (variant.is_type<int>())
                    node[property.get_name()] = variant.convert<int>();
                //else if (variant.is_type<uint64_t>())
                    
            }
            return node;
        }
    };
}
