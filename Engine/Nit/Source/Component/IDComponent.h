#pragma once

namespace Nit
{
    struct IDComponent
    {
        Id ID;

        IDComponent()
            : ID(0)
        {}

        IDComponent(Id id)
            : ID(id)
        {}

        IDComponent(uint64_t id)
            : ID(id)
        {}

        bool operator ==(const IDComponent& other) const { return ID == other.ID; }
        bool operator !=(const IDComponent& other) const { return ID != other.ID; }
        bool operator !() const { return ID == 0; }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(IDComponent)
}