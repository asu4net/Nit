#include "Asset.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    registration::class_<Asset>("Asset")
        .constructor<>()
        .constructor<const std::string&, const std::string&>()
        .property("Name", &Asset::m_Name)
        .property("Name", &Asset::m_Path)
        .property("Name", &Asset::m_AbsolutePath);
}

namespace Nit
{
    Asset::Asset(const std::string& name, const std::string& path)
        : m_Name(name)
        , m_Path(path)
        , m_AbsolutePath(CurrentDirectory() + "\\" + path)
    {
    }
}