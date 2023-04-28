#include "NitPCH.h"
#include "Asset.h"

#include <utility>

RTTR_REGISTRATION
{
    rttr::registration::class_<Nit::Asset>("Asset")
        .constructor<>()
        .constructor<const std::string&, const std::string&, Nit::Id>()
        .property("name", &Nit::Asset::m_Name)
        .property("path", &Nit::Asset::m_Path)
        .property("id", &Nit::Asset::m_Id);
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
        , m_Id(std::move(id))
    {
    }
}