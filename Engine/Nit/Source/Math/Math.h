#pragma once
#include "Core/Base.h"

namespace Nit
{
    /////////////////////////
    /// COLORS
    ////////////////////////

    constexpr Vec4 Black = {0, 0, 0, 1};
    constexpr Vec4 Blue = {0, 0, 1, 1};
    constexpr Vec4 LightBlue = {.3f, .3f, 1, 1};
    constexpr Vec4 Clear = {0, 0, 0, 0};
    constexpr Vec4 Cyan = {0, 1, 1, 1};
    constexpr Vec4 Gray = {.5f, .5f, .5f, 1};
    constexpr Vec4 Green = {0, 1, 0, 1};
    constexpr Vec4 LightGreen = {.3f, 1, .3f, 1};
    constexpr Vec4 Grey{.2f, .2f, .2f, 1};
    constexpr Vec4 DarkGrey = {.13f, .13f, .13f, 1};
    constexpr Vec4 Magenta = {1, 0, 1, 1};
    constexpr Vec4 Red = {1, 0, 0, 1};
    constexpr Vec4 LightRed = {1, .3f, .3f, 1};
    constexpr Vec4 White = {1, 1, 1, 1};
    constexpr Vec4 Yellow = {1, .92f, .016f, 1};

    /////////////////////////
    /// VECTORS
    ////////////////////////

    constexpr Vec3 VecZero = {0, 0, 0};
    constexpr Vec3 VecOne = {1, 1, 1};
    constexpr Vec3 VecRight = {1, 0, 0};
    constexpr Vec3 VecLeft = {-1, 0, 0};
    constexpr Vec3 VecUp = {0, 1, 0};
    constexpr Vec3 VecDown = {0, -1, 0};
    constexpr Vec3 VecBack = {0, 0, -1};
    constexpr Vec3 VecForward = {0, 0, 1};

    /////////////////////////
    /// QUATERNIONS
    ////////////////////////

    constexpr Quat QuatIdentity = {1, 0, 0, 0};

    /////////////////////////
    /// MATRICES
    ////////////////////////

    constexpr Mat4 MatIdentity = Mat4(1);

    ////////////////////////
    /// REFLECTION
    ////////////////////////

    namespace MathRegistration
    {
        RTTR_REGISTRATION
        {
            rttr::registration::class_<Vec2>("vec2")
                .constructor<>()
                .constructor<float, float>()
                .property("x", &Vec2::x)
                .property("y", &Vec2::y);

            rttr::registration::class_<Vec3>("vec3")
                .constructor<>()
                .constructor<float, float, float>()
                .property("x", &Vec3::x)
                .property("y", &Vec3::y)
                .property("z", &Vec3::z);

            rttr::registration::class_<Vec4>("vec4")
                .constructor<>()
                .constructor<float, float, float, float>()
                .property("x", &Vec4::x)
                .property("y", &Vec4::y)
                .property("z", &Vec4::z)
                .property("w", &Vec4::w);

            rttr::registration::class_<Quat>("quat")
                .constructor<>()
                .constructor<float, float, float, float>()
                .property("x", &Quat::x)
                .property("y", &Quat::y)
                .property("z", &Quat::z)
                .property("w", &Quat::w);
        }
    }
}
