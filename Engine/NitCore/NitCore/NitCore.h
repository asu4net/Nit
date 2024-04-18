#pragma once

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
#include <Windows.h>
#include <algorithm>
#include <thread>
#include <utility>
#include <random>
#include <cstdint>

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

    using TString = std::string;
    using TStringStream = std::stringstream;
    using TIStringStream = std::istringstream;

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

    template<typename T>
    using TFunctionPtr = std::function<T>;
}