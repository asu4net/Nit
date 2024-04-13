#pragma once

#ifndef NIT_STD_CONTAINERS
#define NIT_STD_CONTAINERS
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <functional>
#include <cassert>
#endif

#ifndef NIT_TYPE_ALIASES
#define NIT_TYPE_ALIASES
namespace Nit
{
    template<typename T, size_t N>
    using Array = std::array<T, N>;

    template<typename T>
    using DynamicArray = std::vector<T>;

    template<typename K, typename V>
    using OrderedMap = std::map<K, V>;

    template<typename K, typename V>
    using Map = std::unordered_map<K, V>;

    using String = std::string;
    using StringStream = std::stringstream;
    using IStringStream = std::istringstream;

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr UniquePtr<T> CreateUniquePtr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr SharedPtr<T> CreateSharedPtr(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    template<typename T>
    using FunctionPtr = std::function<T>;
}
#endif

// External
#include <NitMath/NitMath.h>

#ifndef NIT_RTTR
#define NIT_RTTR

#include <RTTR/registration_friend.h>
#include <RTTR/registration>
#include <RTTR/type>

namespace Nit
{
    using Type = rttr::type;
    using Instance = rttr::instance;
    using Variant = rttr::variant;
}

// Try to send to your graphic API an element with a vtable pointer and see what happens
#define RTTR_ENABLE_NO_VIRTUAL \
public:\
RTTR_BEGIN_DISABLE_OVERRIDE_WARNING \
RTTR_INLINE ::rttr::type get_type() const { return ::rttr::detail::get_type_from_instance(this); }  \
RTTR_INLINE void* get_ptr() { return reinterpret_cast<void*>(this); } \
RTTR_INLINE ::rttr::detail::derived_info get_derived_info() { return {reinterpret_cast<void*>(this), ::rttr::detail::get_type_from_instance(this)}; } \
RTTR_END_DISABLE_OVERRIDE_WARNING \
private:

#define NIT_FORCE_LINK(_TYPE) \
struct ForceLink##_TYPE##Class { ForceLink##_TYPE##Class(); }; \
inline ForceLink##_TYPE##Class g_ForceLink##_TYPE##Variable;

#define NIT_FORCE_LINK_IMPL(_TYPE) \
ForceLink##_TYPE##Class::ForceLink##_TYPE##Class() { _TYPE _TYPE##ForceLink; };

#endif

// Internal
#include "Core/CommonEnums.h"
#include "Core/Utils.h"