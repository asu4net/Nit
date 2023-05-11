#include "NitPCH.h"
#include "SceneSystem.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<SceneSystem>("SceneSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&, const Weak<entt::registry>&>();
}