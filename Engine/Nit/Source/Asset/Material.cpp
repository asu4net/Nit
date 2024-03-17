#include "Material.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(Material);
}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<Material>("Material")
        .constructor<>()
        .property("ShaderRef", &Material::m_ShaderRef);
}

namespace Nit
{
    bool Material::Load()
    {
        return true;
    }

    void Material::Unload()
    {
    }
}