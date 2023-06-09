﻿#include "Asset.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    registration::class_<Asset>("Asset")
        .constructor<>()
        .constructor<const std::string&, const std::string&, Id&>()
        .property("Name", &Asset::m_Name)
        .property("Path", &Asset::m_Path)
        .property("Id", &Asset::m_Id)
        .property("TypeName", &Asset::m_TypeName);
}

namespace Nit
{
    Asset::Asset(const std::string& name, const std::string& path, const Id& id)
        : m_Name(name)
        , m_Path(path.empty() ? "None" : path)
        , m_AbsolutePath(path.empty() ? "None" : CurrentDirectory() + "\\" + path)
        , m_Id(id)
    {
    }
}