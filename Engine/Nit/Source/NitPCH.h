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
    using Array = std::array<T, N>;

    template<typename T>
    using DynamicArray = std::vector<T>;

    template<typename K, typename V>
    using OrderedMap = std::map<K, V>;

    template<typename K, typename V>
    using Map = std::unordered_map<K, V>;
}

#include <string>
#include <sstream>
#include <fstream>

namespace Nit
{
    using String = std::string;
    using StringStream = std::stringstream;
    using IStringStream = std::istringstream;
}

#include <memory>

namespace Nit
{
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

#include "NitMath.h"

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