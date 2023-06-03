#pragma once

namespace Nit
{
    enum class Flip
    {
        None, X, Y, Both
    };
}

namespace internal_enums
{
    RTTR_REGISTRATION
    {
        using namespace rttr;
        using namespace Nit;
    
        registration::enumeration<Flip>("Flip")
        (
            value("None", Flip::None),
            value("X", Flip::X),
            value("Y", Flip::Y),
            value("Both", Flip::Both)
        );
    }
}
