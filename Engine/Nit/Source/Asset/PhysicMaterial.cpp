#include "NitPCH.h"
#include "PhysicMaterial.h"
#include <alc/alcmain.h>

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::class_<PhysicMaterial>("PhysicMaterial")
       .constructor<>()
       .property("Density",             &PhysicMaterial::Density)
       .property("Friction",            &PhysicMaterial::Friction)
       .property("Bounciness",          &PhysicMaterial::Bounciness)
       .property("BouncinessThreshold", &PhysicMaterial::BouncinessThreshold);
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(PhysicMaterial)
}