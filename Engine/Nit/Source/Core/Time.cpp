#include "Time.h"
#include "GLFW/glfw3.h"

//TODO: Implement target frame rate

namespace Nit
{
    double Time::MaxDeltaTime = 1.f / 15.f;
    double Time::FixedDeltaTime = 0.0166f;
    
    bool Time::IsInitialized()
    {
        return m_IsInitalized;
    }

    float Time::GetDeltaTime()
    {
       return static_cast<float>(m_DeltaTime);
    }

    float Time::GetApplicationTime()
    {
        return static_cast<float>(m_ApplicationTime);
    }

    uint32_t Time::GetFrameRate()
    {
        return static_cast<uint32_t>((static_cast<double>(m_ApplicationFrames) / m_ApplicationTime));
    }

    uint32_t Time::GetApplicationFrames()
    {
        return m_ApplicationFrames;
    }

    uint32_t Time::GetFixedUpdateCalls()
    {
        return m_FixedUpdateCalls;
    }

    void Time::Init()
    {
        m_IsInitalized = true;
        m_DeltaTime = 0;
        m_ApplicationTime = 0;
        m_ApplicationFrames = 0;
        m_FixedUpdateCalls = 0;
        m_FixedUpdateTimer = 0;

        m_LastTime = glfwGetTime();

        NIT_LOG_TRACE("Time initialized!\n");
    }

    void Time::CalculateTimeStep()
    {
        if (!m_IsInitalized)
        {
            NIT_CHECK(false, "Should call Init() first");
            Init();
        }

        m_FixedUpdateCalls = 0;
        m_ApplicationFrames++;

        double time = glfwGetTime();
        const double timeBetweenFrames = time - m_LastTime;
        m_LastTime = time;

        m_ApplicationTime += timeBetweenFrames;
        m_DeltaTime = std::clamp(timeBetweenFrames, 0., MaxDeltaTime);
        m_FixedUpdateTimer += m_DeltaTime;

        while (m_FixedUpdateTimer >= FixedDeltaTime)
        {
            m_FixedUpdateCalls++;
            m_FixedUpdateTimer -= FixedDeltaTime;
        }
    }
}