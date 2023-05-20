#include "ActorComponent.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<ActorComponent>("ActorComponent")
        .constructor<>();
}