#include "Id.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<Id>("Id")
        .constructor<>()(rttr::policy::ctor::as_object)
        .constructor<uint64_t>()(rttr::policy::ctor::as_object)
        .property("IdHandler", &Id::m_IdHandler);
}

namespace Nit
{
    //Idk wtf is this, but it generates a random number.
    static std::random_device g_RandomDevice;
    static std::mt19937_64 g_RandomEngine(g_RandomDevice());
    static std::uniform_int_distribution<uint64_t> g_Distribution(g_RandomDevice());
    
    Id::Id()
        : m_IdHandler(g_Distribution(g_RandomEngine))
    {
    }

    Id::Id(const uint64_t id)
        : m_IdHandler(id)
    {
    }

    bool Id::operator==(const Id& other) const
    {
        return m_IdHandler == other.m_IdHandler;
    }

    bool Id::operator!=(const Id& other) const
    {
        return m_IdHandler != other.m_IdHandler;
    }
}