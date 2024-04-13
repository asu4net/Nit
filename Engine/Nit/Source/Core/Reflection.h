#pragma once
#include <RTTR/registration_friend.h>
#include <RTTR/registration>
#include <RTTR/type>
#include "NitMath/NitMath.h"

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

namespace MathRegistration
{
    RTTR_REGISTRATION
    {
        using namespace Nit;
        
        rttr::registration::class_<Color>("Color")
        .constructor<>()
        .property("r", &Color::r)
        .property("g", &Color::g)
        .property("b", &Color::b)
        .property("a", &Color::a);

        rttr::registration::class_<Matrix4>("Matrix4")
        .constructor<>();

        rttr::registration::class_<Vector2>("Vector2")
        .constructor<>()
        .property("x", &Vector2::x)
        .property("y", &Vector2::y);
        
        rttr::registration::class_<Vector3>("Vector3")
        .constructor<>()
        .property("x", &Vector3::x)
        .property("y", &Vector3::y)
        .property("z", &Vector3::z);

        rttr::registration::class_<Vector4>("Vector4")
        .constructor<>()
        .property("x", &Vector4::x)
        .property("y", &Vector4::y)
        .property("z", &Vector4::z)
        .property("w", &Vector4::w);
    }
}