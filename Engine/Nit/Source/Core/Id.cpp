#include "Id.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<Nit::Id>("Id")
        .constructor<>()
        .constructor<uint64_t>()
        .property("id", &Nit::Id::m_Id);
}

namespace Nit
{
    //Idk wtf is this, but it generates a random number.
    static std::random_device g_RandomDevice;
    static std::mt19937_64 g_RandomEngine(g_RandomDevice());
    static std::uniform_int_distribution<uint64_t> g_Distribution(g_RandomDevice());
    
    Id::Id()
        : m_Id(g_Distribution(g_RandomEngine))
    {
    }

    Id::Id(const uint64_t id)
        : m_Id(id)
    {
    }

    bool Id::operator==(const Id& other) const
    {
        return m_Id == other.m_Id;
    }

    bool Id::operator!=(const Id& other) const
    {
        return m_Id != other.m_Id;
    }
}