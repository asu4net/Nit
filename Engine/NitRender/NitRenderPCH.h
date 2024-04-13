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

// Internal
#include "Core/CommonEnums.h"
#include "Core/Utils.h"