#pragma once
#include <yaml-cpp/yaml.h>

namespace Nit
{
    class YAMLSerializer
    {
    public:
        void SerializeObject(const rttr::instance& object, const std::string& name, YAML::Emitter& out)
        {
            const rttr::type t = object.get_type();
            if (!t.is_valid()) return;
            out << YAML::Key << name << YAML::Value << YAML::BeginMap;
            
            for (const auto& property : t.get_properties())
            {
                const rttr::variant variant = property.get_value(object);
                const rttr::type tProp = property.get_type();
                if (tProp.is_class()
                    && !variant.is_type<std::string>()
                    && !variant.is_type<Id>()
                    && !variant.is_type<uint32_t>()
                    && !variant.is_type<uint64_t>())
                {
                    SerializeObject(variant, std::string(property.get_name()), out);
                    continue;
                }

                //Serialize property
                if      (variant.is_type<char>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<char>();
                else if (variant.is_type<bool>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<bool>();
                else if (variant.is_type<int>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<int>();
                else if (variant.is_type<Id>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << static_cast<uint64_t>(variant.convert<Id>());
                else if (variant.is_type<uint32_t>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<uint32_t>();
                else if (variant.is_type<uint64_t>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<uint64_t>();
                else if (variant.is_type<float>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<float>();
                else if (variant.is_type<double>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<double>();
                else if (variant.is_type<std::string>())
                    out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<std::string>();
            }
        }
    };
}
