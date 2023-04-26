#include "NitPCH.h"
#include "Id.h"
#include <random>

namespace Nit
{
    //Idk wtf is this, but it works
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