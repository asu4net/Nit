#pragma once
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

