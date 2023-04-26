#pragma once

namespace Nit
{
    inline std::string CurrentDirectory()
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        const std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }

    template<typename T>
    using Unique = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Unique<T> CreateUnique(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Shared = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Shared<T> CreateShared(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Weak = std::weak_ptr<T>;
}