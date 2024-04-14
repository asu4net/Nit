#pragma once

namespace Nit
{
    struct NameComponent
    {
        TString Name;

        NameComponent() = default;
        
        NameComponent(const TString& name)
            : Name(name)
        {}

        NameComponent(const char* name)
            : Name(name)
        {}

        bool IsEmpty() const { return Name.empty(); }

        bool Contains(const TString& other) const { return Name.find(other) != TString::npos; }
        bool Contains(const char* other) const { return Name.find(other) != TString::npos; }
        const char* GetRawString() const { return Name.c_str(); }

        bool operator ==(const NameComponent& other) const { return Name == other.Name; }
        bool operator !=(const NameComponent& other) const { return Name != other.Name; }
        bool operator !() const { return IsEmpty(); }
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(NameComponent)
}