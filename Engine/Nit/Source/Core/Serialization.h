#pragma once

namespace Nit::Serialization
{
    void SerializeObject(
            const rttr::instance& object,
            const std::string& name,
            std::string& result);
}