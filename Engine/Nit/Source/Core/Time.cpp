#include "Time.h"

namespace Nit
{
    Time::Time()
    {
        m_FixedDeltaTime = 1 / m_TargetFrameRate;
    
        m_LastTime = GetTime();

        LARGE_INTEGER frec;
        QueryPerformanceFrequency(&frec);
        m_Frec = frec;
    }

    TimeStep Time::CalculateTimeStep()
    {
        const LARGE_INTEGER time = GetTime();
    
        const double timeBetweenFrames = static_cast<double>(time.QuadPart - m_LastTime.QuadPart) /
            static_cast<double>(m_Frec.QuadPart);
    
        m_LastTime = time;

        m_GameTime += timeBetweenFrames;
    
        m_Elapsed += timeBetweenFrames;
        m_Elapsed = std::clamp(m_Elapsed, 0., static_cast<double>(MaxDeltaTime));

        m_DeltaTime = m_Elapsed;
        TimeStep timeStep = { GetDeltaTime(), GetFixedDeltaTime(), GetGameTime() };
        
        while (m_Elapsed >= m_FixedDeltaTime)
        {
            timeStep.FixedUpdateCalls++;
            m_Elapsed -= m_FixedDeltaTime;
        }

        return timeStep;
    }

    LARGE_INTEGER Time::GetTime()
    {
        LARGE_INTEGER ticks;
        QueryPerformanceCounter(&ticks);
        return ticks;
    }
}
