#include "Serialization.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/document.h>     // rapidjson's DOM-style API

using namespace rapidjson;
using namespace rttr;

namespace Nit::Serialization::Internal
{

    /////////////////////////////////////////////////////////////////////////////////////////

    void ToJsonRecursively(const instance& targetObject, PrettyWriter<StringBuffer>& writer);

    /////////////////////////////////////////////////////////////////////////////////////////

    bool WriteVariant(const variant& var, PrettyWriter<StringBuffer>& writer);

    bool WriteAtomicTypesToJson(const type& t, const variant& var, PrettyWriter<StringBuffer>& writer)
    {
        if (t.is_arithmetic())
        {
            if (t == type::get<bool>())
                writer.Bool(var.to_bool());
            else if (t == type::get<char>())
                writer.Bool(var.to_bool());
            else if (t == type::get<int8_t>())
                writer.Int(var.to_int8());
            else if (t == type::get<int16_t>())
                writer.Int(var.to_int16());
            else if (t == type::get<int32_t>())
                writer.Int(var.to_int32());
            else if (t == type::get<int64_t>())
                writer.Int64(var.to_int64());
            else if (t == type::get<uint8_t>())
                writer.Uint(var.to_uint8());
            else if (t == type::get<uint16_t>())
                writer.Uint(var.to_uint16());
            else if (t == type::get<uint32_t>())
                writer.Uint(var.to_uint32());
            else if (t == type::get<uint64_t>())
                writer.Uint64(var.to_uint64());
            else if (t == type::get<float>())
                writer.Double(var.to_double());
            else if (t == type::get<double>())
                writer.Double(var.to_double());

            return true;
        }
        else if (t.is_enumeration())
        {
            bool ok = false;
            auto result = var.to_string(&ok);
            if (ok)
            {
                writer.String(var.to_string());
            }
            else
            {
                ok = false;
                const auto value = var.to_uint64(&ok);
                if (ok)
                    writer.Uint64(value);
                else
                    writer.Null();
            }

            return true;
        }
        else if (t == type::get<std::string>())
        {
            writer.String(var.to_string());
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static void WriteArray(const variant_sequential_view& view, PrettyWriter<StringBuffer>& writer)
    {
        writer.StartArray();
        for (const auto& item : view)
        {
            if (item.is_sequential_container())
            {
                WriteArray(item.create_sequential_view(), writer);
            }
            else
            {
                variant wrapped_var = item.extract_wrapped_value();
                type value_type = wrapped_var.get_type();
                if (value_type.is_arithmetic() || value_type == type::get<std::string>() || value_type.is_enumeration())
                {
                    WriteAtomicTypesToJson(value_type, wrapped_var, writer);
                }
                else // object
                {
                    ToJsonRecursively(wrapped_var, writer);
                }
            }
        }
        writer.EndArray();
    }


    /////////////////////////////////////////////////////////////////////////////////////////

    static void WriteAssociativeContainer(const variant_associative_view& view, PrettyWriter<StringBuffer>& writer)
    {
        static constexpr string_view KeyName("key");
        static constexpr string_view ValueName("value");

        writer.StartArray();

        if (view.is_key_only_type())
        {
            for (auto& item : view)
            {
                WriteVariant(item.first, writer);
            }
        }
        else
        {
            for (auto& item : view)
            {
                writer.StartObject();
                writer.String(KeyName.data(), static_cast<rapidjson::SizeType>(KeyName.length()), false);

                WriteVariant(item.first, writer);

                writer.String(ValueName.data(), static_cast<rapidjson::SizeType>(ValueName.length()), false);

                WriteVariant(item.second, writer);

                writer.EndObject();
            }
        }

        writer.EndArray();
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    bool WriteVariant(const variant& var, PrettyWriter<StringBuffer>& writer)
    {
        const auto valueType = var.get_type();
        const auto wrappedType = valueType.is_wrapper() ? valueType.get_wrapped_type() : valueType;
        const bool isWrapper = wrappedType != valueType;

        if (WriteAtomicTypesToJson(isWrapper ? wrappedType : valueType,
            isWrapper ? var.extract_wrapped_value() : var, writer))
        {
        }
        else if (var.is_sequential_container())
        {
            WriteArray(var.create_sequential_view(), writer);
        }
        else if (var.is_associative_container())
        {
            WriteAssociativeContainer(var.create_associative_view(), writer);
        }
        else
        {
            const auto childProps = isWrapper ? wrappedType.get_properties() : valueType.get_properties();
            if (!childProps.empty())
            {
                ToJsonRecursively(var, writer);
            }
            else
            {
                bool ok = false;
                const auto text = var.to_string(&ok);
                if (!ok)
                {
                    writer.String(text);
                    return false;
                }

                writer.String(text);
            }
        }

        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    void ToJsonRecursively(const instance& targetObject, PrettyWriter<StringBuffer>& writer)
    {
        writer.StartObject();
        const instance object = targetObject.get_type().get_raw_type().is_wrapper() ? targetObject.get_wrapped_instance() : targetObject;

        auto propList = object.get_derived_type().get_properties();
        for (auto prop : propList)
        {
            if (prop.get_metadata("NO_SERIALIZE"))
                continue;

            variant propValue = prop.get_value(object);
            if (!propValue)
                continue; // cannot serialize, because we cannot retrieve the value

            const auto name = prop.get_name();
            writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);
            if (!WriteVariant(propValue, writer))
            {
                std::cerr << "cannot serialize property: " << name << std::endl;
            }
        }

        writer.EndObject();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////

    void FromJsonRecursively(const instance& obj, Value& jsonObject);

    /////////////////////////////////////////////////////////////////////////////////////////

    variant ExtractBasicTypes(const Value& jsonValue)
    {
        switch (jsonValue.GetType())
        {
        case kStringType:
        {
            return std::string(jsonValue.GetString());
        }
        case kNullType:     break;
        case kFalseType:
        case kTrueType:
        {
            return jsonValue.GetBool();
        }
        case kNumberType:
        {
            if (jsonValue.IsInt())
                return jsonValue.GetInt();
            else if (jsonValue.IsDouble())
                return jsonValue.GetDouble();
            else if (jsonValue.IsUint())
                return jsonValue.GetUint();
            else if (jsonValue.IsInt64())
                return jsonValue.GetInt64();
            else if (jsonValue.IsUint64())
                return jsonValue.GetUint64();
            break;
        }
        // we handle only the basic types here
        case kObjectType:
        case kArrayType: return {};
        }

        return {};
    }


    /////////////////////////////////////////////////////////////////////////////////////////

    static void write_array_recursively(variant_sequential_view& view, Value& json_array_value)
    {
        view.set_size(json_array_value.Size());
        const type arrayValueType = view.get_rank_type(1);

        for (SizeType i = 0; i < json_array_value.Size(); ++i)
        {
            auto& jsonIndexValue = json_array_value[i];
            if (jsonIndexValue.IsArray())
            {
                auto sub_array_view = view.get_value(i).create_sequential_view();
                write_array_recursively(sub_array_view, jsonIndexValue);
            }
            else if (jsonIndexValue.IsObject())
            {
                variant varTmp = view.get_value(i);
                variant wrappedVar = varTmp.extract_wrapped_value();
                FromJsonRecursively(wrappedVar, jsonIndexValue);
                view.set_value(i, wrappedVar);
            }
            else
            {
                variant extractedValue = ExtractBasicTypes(jsonIndexValue);
                if (extractedValue.convert(arrayValueType))
                    view.set_value(i, extractedValue);
            }
        }
    }

    variant ExtractValue(const Value::MemberIterator& it, const type& t)
    {
        auto& jsonValue = it->value;
        variant extractedValue = ExtractBasicTypes(jsonValue);
        const bool couldConvert = extractedValue.convert(t);
        if (!couldConvert)
        {
            if (jsonValue.IsObject())
            {
                constructor ctor = t.get_constructor();
                for (auto& item : t.get_constructors())
                {
                    if (item.get_instantiated_type() == t)
                        ctor = item;
                }
                extractedValue = ctor.invoke();
                FromJsonRecursively(extractedValue, jsonValue);
            }
        }

        return extractedValue;
    }

    static void WriteAssociativeViewRecursively(variant_associative_view& view, Value& jsonArrayValue)
    {
        for (SizeType i = 0; i < jsonArrayValue.Size(); ++i)
        {
            auto& jsonIndexValue = jsonArrayValue[i];
            if (jsonIndexValue.IsObject()) // a key-value associative view
            {
                Value::MemberIterator keyItr = jsonIndexValue.FindMember("key");
                Value::MemberIterator valueItr = jsonIndexValue.FindMember("value");

                if (keyItr != jsonIndexValue.MemberEnd() &&
                    valueItr != jsonIndexValue.MemberEnd())
                {
                    auto keyVar = ExtractValue(keyItr, view.get_key_type());
                    auto valueVar = ExtractValue(valueItr, view.get_value_type());
                    if (keyVar && valueVar)
                    {
                        view.insert(keyVar, valueVar);
                    }
                }
            }
            else // a key-only associative view
            {
                variant extractedValue = ExtractBasicTypes(jsonIndexValue);
                if (extractedValue && extractedValue.convert(view.get_key_type()))
                    view.insert(extractedValue);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    void FromJsonRecursively(const instance& obj, Value& jsonObject)
    {
        const instance inst = obj.get_type().get_raw_type().is_wrapper() ? obj.get_wrapped_instance() : obj;
        const auto propList = inst.get_derived_type().get_properties();

        for (auto prop : propList)
        {
            Value::MemberIterator ret = jsonObject.FindMember(prop.get_name().data());
            if (ret == jsonObject.MemberEnd())
                continue;
            const type valueT = prop.get_type();

            auto& jsonValue = ret->value;
            switch (jsonValue.GetType())  // NOLINT(clang-diagnostic-switch-enum)
            {
            case kArrayType:
            {
                variant var;
                if (valueT.is_sequential_container())
                {
                    var = prop.get_value(inst);
                    auto view = var.create_sequential_view();
                    write_array_recursively(view, jsonValue);
                }
                else if (valueT.is_associative_container())
                {
                    var = prop.get_value(inst);
                    auto associative_view = var.create_associative_view();
                    WriteAssociativeViewRecursively(associative_view, jsonValue);
                }

                prop.set_value(inst, var);
                break;
            }
            case kObjectType:
            {
                variant var = prop.get_value(inst);
                FromJsonRecursively(var, jsonValue);
                prop.set_value(inst, var);
                break;
            }
            default:
            {
                variant extractedValue = ExtractBasicTypes(jsonValue);
                if (extractedValue.convert(valueT)) // REMARK: CONVERSION WORKS ONLY WITH "const type", check whether this is correct or not!
                    prop.set_value(inst, extractedValue);
            }
            }
        }
    }

}

namespace Nit::Serialization
{
    bool FromJson(const std::string& json, const rttr::instance& object)
    {
        Document document; // Default template parameter uses UTF8 and MemoryPoolAllocator.

        // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
        if (document.Parse(json.c_str()).HasParseError())
            return true;

        Internal::FromJsonRecursively(object, document);

        return true;
    }

    std::string ToJson(const rttr::instance& object)
    {
        if (!object.is_valid())
            return {};

        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);

        Internal::ToJsonRecursively(object, writer);

        return sb.GetString();
    }
}