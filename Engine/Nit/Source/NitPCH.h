#pragma once

/// --------------------

#include <entt/entt.hpp>

using Registry = entt::registry;
using RawEntity = entt::entity;

#include "Core/Logging.h"
#include "Core/Singleton.h"

#include <array>
#include <vector>
#include <map>
#include <unordered_map>

namespace Nit
{
    template<typename T, size_t N>
    using TArray = std::array<T, N>;

    template<typename T>
    using TDynamicArray = std::vector<T>;

    template<typename K, typename V>
    using TOrderedMap = std::map<K, V>;

    template<typename K, typename V>
    using TMap = std::unordered_map<K, V>;
}

#include <string>
#include <sstream>
#include <fstream>

namespace Nit
{
    using TString = std::string;
    using TStringStream = std::stringstream;
    using TIStringStream = std::istringstream;
}

#include <memory>

namespace Nit
{
    template<typename T>
    using TUniquePtr = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr TUniquePtr<T> CreateUniquePtr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using TSharedPtr = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr TSharedPtr<T> CreateSharedPtr(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using TWeakPtr = std::weak_ptr<T>;
}

#include "Core/MulticastDelegate.h"
#include "Core/Delegate.h"
#include "Core/Reflection.h"
#include "Core/File.h"
#include "Core/Enums.h"
#include "Core/Id.h"
#include "Core/Directory.h"
#include "Core/Serialization.h"
#include "Core/Time.h"

#include "NitMath/NitMath.h"

#include "Input/KeyCodes.h"
#include "Input/Input.h"
#include "Input/InputAction.h"

/// --------------------

#include <Windows.h>
#include <algorithm>
#include <thread>
#include <utility>
#include <random>

/// --------------------

// #ifndef NIT_RTTR
// #define NIT_RTTR
//
// #include <RTTR/registration_friend.h>
// #include <RTTR/registration>
// #include <RTTR/type>
//
// namespace Nit
// {
//     using Type = rttr::type;
//     using Instance = rttr::instance;
//     using Variant = rttr::variant;
// }
//
// // Try to send to your graphic API an element with a vtable pointer and see what happens
// #define NIT_RTTR_ENABLE_NO_VIRTUAL \
// public:\
// RTTR_INLINE ::rttr::type GetType() const { return ::rttr::detail::get_type_from_instance(this); }  \
// private:
//
// #define NIT_TYPE_LIST(...)      ::rttr::type_list<__VA_ARGS__>
//
// #define NIT_RTTR_ENABLE(...) \
// public:\
// RTTR_BEGIN_DISABLE_OVERRIDE_WARNING \
// virtual RTTR_INLINE ::rttr::type GetType() const { return ::rttr::detail::get_type_from_instance(this); }  \
// virtual RTTR_INLINE void* GetPtr() { return reinterpret_cast<void*>(this); } \
// virtual RTTR_INLINE ::rttr::detail::derived_info GetDerivedInfo() { return {reinterpret_cast<void*>(this), ::rttr::detail::get_type_from_instance(this)}; } \
// using base_class_list = NIT_TYPE_LIST(__VA_ARGS__); \
// RTTR_END_DISABLE_OVERRIDE_WARNING \
// private:
//
// #define NIT_FORCE_LINK(_TYPE) \
// struct ForceLink##_TYPE##Class { ForceLink##_TYPE##Class(); }; \
// inline ForceLink##_TYPE##Class g_ForceLink##_TYPE##Variable;
//
// #define NIT_FORCE_LINK_IMPL(_TYPE) \
// ForceLink##_TYPE##Class::ForceLink##_TYPE##Class() { _TYPE _TYPE##ForceLink; };
//
// #endif