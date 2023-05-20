#include "Enums.h"

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