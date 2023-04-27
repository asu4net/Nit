#pragma once

namespace Nit
{
    class Id
    {
    public:
        Id();
        Id(uint64_t id);
        Id(Id& other) = default;
        
        explicit operator uint64_t() const { return m_Id; }
        bool operator ==(const Id& other) const;
        bool operator !=(const Id& other) const;
        
    private:
        uint64_t m_Id;

        RTTR_REGISTRATION_FRIEND
        RTTR_ENABLE()
    };
}

template<>
struct std::hash<Nit::Id>
{
    std::size_t operator()(const Nit::Id id) const noexcept
    {
        return hash<uint64_t>()(static_cast<uint64_t>(id));
    }
};