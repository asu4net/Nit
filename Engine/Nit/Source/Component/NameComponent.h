#pragma once

namespace Nit
{
    struct NameComponent
    {
        String Name;

        NameComponent() = default;
        
        NameComponent(const String& name)
            : Name(name)
        {}

        NameComponent(const char* name)
            : Name(name)
        {}

        bool IsEmpty() const { return Name.empty(); }

        bool Contains(const String& other) const { return Name.find(other) != String::npos; }
        bool Contains(const char* other) const { return Name.find(other) != String::npos; }
        const char* GetRawString() const { return Name.c_str(); }

        bool operator ==(const NameComponent& other) const { return Name == other.Name; }
        bool operator !=(const NameComponent& other) const { return Name != other.Name; }
        bool operator !() const { return IsEmpty(); }
        
        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(NameComponent)
}