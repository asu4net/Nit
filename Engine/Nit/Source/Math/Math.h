#pragma once
#include "Core/Base.h"

namespace Nit
{
    inline vec3 lookAt(const quat& rot, const vec3& axis)
    {
        const mat4 matRot = toMat4(rot);
        const vec4 dir = vec4(axis, 1);
        return {matRot * dir};
    }

    /////////////////////////
    /// COLORS
    ////////////////////////

    constexpr vec4 Black = {0, 0, 0, 1};
    constexpr vec4 Blue = {0, 0, 1, 1};
    constexpr vec4 LightBlue = {.3f, .3f, 1, 1};
    constexpr vec4 Clear = {0, 0, 0, 0};
    constexpr vec4 Cyan = {0, 1, 1, 1};
    constexpr vec4 Gray = {.5f, .5f, .5f, 1};
    constexpr vec4 Green = {0, 1, 0, 1};
    constexpr vec4 LightGreen = {.3f, 1, .3f, 1};
    constexpr vec4 Grey{.2f, .2f, .2f, 1};
    constexpr vec4 DarkGrey = {.13f, .13f, .13f, 1};
    constexpr vec4 Magenta = {1, 0, 1, 1};
    constexpr vec4 Red = {1, 0, 0, 1};
    constexpr vec4 LightRed = {1, .3f, .3f, 1};
    constexpr vec4 White = {1, 1, 1, 1};
    constexpr vec4 Yellow = {1, .92f, .016f, 1};

    /////////////////////////
    /// VECTORS
    ////////////////////////

    constexpr vec3 VecZero = {0, 0, 0};
    constexpr vec3 VecOne = {1, 1, 1};
    constexpr vec3 VecRight = {1, 0, 0};
    constexpr vec3 VecLeft = {-1, 0, 0};
    constexpr vec3 VecUp = {0, 1, 0};
    constexpr vec3 VecDown = {0, -1, 0};
    constexpr vec3 VecBack = {0, 0, -1};
    constexpr vec3 VecForward = {0, 0, 1};

    /////////////////////////
    /// QUATERNIONS
    ////////////////////////

    constexpr quat QuatIdentity = {1, 0, 0, 0};

    /////////////////////////
    /// MATRICES
    ////////////////////////

    constexpr mat4 MatIdentity = mat4(1);

    ////////////////////////
    /// REFLECTION
    ////////////////////////

    namespace MathRegistration
    {
        RTTR_REGISTRATION
        {
            rttr::registration::class_<vec2>("vec2")
                .constructor<>()
                .constructor<float, float>()
                .property("x", &vec2::x)
                .property("y", &vec2::y);

            rttr::registration::class_<vec3>("vec3")
                .constructor<>()
                .constructor<float, float, float>()
                .property("x", &vec3::x)
                .property("y", &vec3::y)
                .property("z", &vec3::z);

            rttr::registration::class_<vec4>("vec4")
                .constructor<>()
                .constructor<float, float, float, float>()
                .property("x", &vec4::x)
                .property("y", &vec4::y)
                .property("z", &vec4::z)
                .property("w", &vec4::w);

            rttr::registration::class_<quat>("quat")
                .constructor<>()
                .constructor<float, float, float, float>()
                .property("x", &quat::x)
                .property("y", &quat::y)
                .property("z", &quat::z)
                .property("w", &quat::w);
        }
    }
}
