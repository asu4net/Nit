#include "NitPCH.h"
#include "Asset.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<Nit::Asset>("Asset")
        .constructor<>()
        .constructor<const std::string&, const std::string&, Nit::Id>();
}

namespace Nit
{
    Asset::Asset()
        : m_Id(0)
    {
    }

    Asset::Asset(const std::string& name, const std::string& path, Id id)
        : m_Name(name)
        , m_Path(path)
        , m_Id(id)
    {
    }
}