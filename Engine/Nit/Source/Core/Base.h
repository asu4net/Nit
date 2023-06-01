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

#define NIT_EXECUTION_ORDER(_ORDER) \
    int GetExecutionOrder() const override { return _ORDER; }
    
#define NIT_FORCE_LINK(_TYPE) \
    struct ForceLink##_TYPE##Class { ForceLink##_TYPE##Class(); }; \
    inline ForceLink##_TYPE##Class g_ForceLink##_TYPE##Variable;

#define NIT_REGISTRY_COMPONENT(_TYPE) \
    Scene::ComponentMetaData[type::get<_TYPE>()] = { \
    Delegate<bool(Actor)>([](const Actor& actor) -> bool \
    { \
        return actor.Has<_TYPE>(); \
    }), \
    Delegate<instance(Actor)>([](const Actor& actor) -> instance \
    { \
        return actor.Get<_TYPE>(); \
    }), \
    Delegate<void(Actor, const rttr::instance&)>([](const Actor& actor, const rttr::instance& instance) \
    { \
        actor.Add<_TYPE>(*instance.try_convert<_TYPE>()); \
    }), \
    Delegate<void(Actor)>([](const Actor& actor) \
    { \
        actor.Remove<_TYPE>(); \
    }), \
    Delegate<void(Actor, const rttr::instance&)>([](const Actor& actor, const rttr::instance& instance) \
    { \
        actor.Get<_TYPE>() = *instance.try_convert<_TYPE>(); \
    }) \
};

#define NIT_FORCE_LINK_IMPL(_TYPE) \
    ForceLink##_TYPE##Class::ForceLink##_TYPE##Class() { _TYPE _TYPE##ForceLink; };
    
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;
    using Vec2 = glm::vec2;
    using Mat4 = glm::mat4;
    using Quat = glm::quat;
    
    namespace Math
    {
        using namespace glm;
        
        inline Mat4 Translate(const Mat4& mat, const Vec3& pos)
        {
            return translate(mat, pos);
        }

        inline Mat4 Scale(const Mat4& mat, const Vec3& scale)
        {
            return glm::scale(mat, scale);
        }

        inline Mat4 ToMat4(const Quat& quat)
        {
            return toMat4(quat);
        }

        inline Vec3 LookAt(const Quat& rot, const Vec3& axis)
        {
            const Mat4 matRot = ToMat4(rot);
            const Vec4 dir = Vec4(axis, 1);
            return {matRot * dir};
        }

        inline void Decompose(const Mat4& mat, Vec3& pos, Quat& rot, Vec3& scale)
        {
            Vec3 skew;
            Vec4 persp;
            decompose(mat, scale, rot, pos, skew, persp);
        }
    }
}
