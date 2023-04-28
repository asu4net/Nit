#pragma once
#include <yaml-cpp/yaml.h>

namespace Nit
{
    class YAMLSerializer
    {
    public:
        void SerializeProperty(const rttr::variant& variant, const rttr::property& property, YAML::Emitter& out)
        {
            if      (variant.is_type<char>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<char>();
            else if (variant.is_type<bool>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<bool>();
            else if (variant.is_type<int>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<int>();
            else if (variant.is_type<int32_t>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<int32_t>();
            else if (variant.is_type<int64_t>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<int64_t>();
            else if (variant.is_type<float>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<float>();
            else if (variant.is_type<double>())
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << variant.convert<double>();
            else if (variant.is_type<std::string>())
            {
                std::string jaja = variant.convert<std::string>();
                out << YAML::Key << std::string(property.get_name()) << YAML::Value << jaja;
            }
        }
        
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
                    && !variant.is_type<std::uint32_t>()
                    && !variant.is_type<std::uint64_t>())
                {
                    SerializeObject(variant, std::string(property.get_name()), out);
                    continue;
                }
                SerializeProperty(variant, property, out);
            }
        }
    };
}
