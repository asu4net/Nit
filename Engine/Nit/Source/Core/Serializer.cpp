#include "Serializer.h"
#include <yaml-cpp/yaml.h>

//TODO: Enums
//TODO: Maps
//TODO: Arrays

namespace Nit::Serialization
{
    template<typename T>
    bool TrySerializeVariant(const rttr::variant& variant, const std::string& name, YAML::Emitter& out)
    {
        if (!variant.is_type<T>()) return false;
        out << YAML::Key << name << YAML::Value << variant.convert<T>();
        return true;
    }

    template<>
    bool TrySerializeVariant<Id>(const rttr::variant& variant, const std::string& name, YAML::Emitter& out)
    {
        if (!variant.is_type<Id>()) return false;
        out << YAML::Key << name << YAML::Value << static_cast<uint64_t>(variant.convert<Id>());
        return true;
    }

    template <>
    bool TrySerializeVariant<glm::vec2>(const rttr::variant& variant, const std::string& name, YAML::Emitter& out)
    {
        if (!variant.is_type<glm::vec2>()) return false;
        const glm::vec2 v = variant.convert<glm::vec2>();
        out << YAML::Key << name << YAML::Value << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return true;
    }
    
    template<>
    bool TrySerializeVariant<glm::vec3>(const rttr::variant& variant, const std::string& name, YAML::Emitter& out)
    {
        if (!variant.is_type<glm::vec3>()) return false;
        const glm::vec3 v = variant.convert<glm::vec3>();
        out << YAML::Key << name << YAML::Value << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return true;
    }

    template<>
    bool TrySerializeVariant<glm::vec4>(const rttr::variant& variant, const std::string& name, YAML::Emitter& out)
    {
        if (!variant.is_type<glm::vec4>()) return false;
        const glm::vec4 v = variant.convert<glm::vec4>();
        out << YAML::Key << name << YAML::Value << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return true;
    }
    
    static bool SerializeAsElement(const rttr::variant& variant, const std::string& name,
        YAML::Emitter& out)
    {
        if (TrySerializeVariant<char>       (variant, name, out)) return true;
        if (TrySerializeVariant<bool>       (variant, name, out)) return true;
        if (TrySerializeVariant<int>        (variant, name, out)) return true;
        if (TrySerializeVariant<float>      (variant, name, out)) return true;
        if (TrySerializeVariant<double>     (variant, name, out)) return true;

        if (TrySerializeVariant<Id>         (variant, name, out)) return true;
        if (TrySerializeVariant<uint64_t>   (variant, name, out)) return true;
        if (TrySerializeVariant<uint32_t>   (variant, name, out)) return true;
        if (TrySerializeVariant<std::string>(variant, name, out)) return true;
        if (TrySerializeVariant<glm::vec2>  (variant, name, out)) return true;
        if (TrySerializeVariant<glm::vec3>  (variant, name, out)) return true;
        if (TrySerializeVariant<glm::vec4>  (variant, name, out)) return true;

        return false;
    }

    static void SerializeAsObject(const rttr::instance& object, const std::string& name, YAML::Emitter& out)
    {
        const rttr::type t = object.get_type();
        if (!t.is_valid()) return;
        
        out << YAML::BeginMap;
        out << YAML::Key << name << YAML::Value << YAML::BeginMap;
        
        for (const auto& property : t.get_properties())
        {
            const rttr::variant variant = property.get_value(object);
        
            const std::string propertyName = std::string(property.get_name());
            if (SerializeAsElement(variant, propertyName, out)) continue;
                
            if (property.get_type().is_class())
            {
                SerializeAsObject(variant, propertyName, out);
                continue;
            }
        }
        
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    void SerializeObject(const rttr::instance& object, const std::string& name,
        std::string& result)
    {
        YAML::Emitter out;
        SerializeAsObject(object, name, out);
        result = out.c_str();
    }
}