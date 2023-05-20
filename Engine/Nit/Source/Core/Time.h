#pragma once

namespace Nit
{
    struct TimeStep
    {
        float DeltaTime = 0;
        float FixedDeltaTime = 0;
        float GameTime = 0;
        int FixedUpdateCalls = 0;
    }; 
    
    class Time
    {
    public:
        static constexpr float MaxDeltaTime = 1.f / 15.f;

        Time();
        TimeStep CalculateTimeStep();
    
        float  GetDeltaTime() const { return static_cast<float>(m_DeltaTime); }
        float  GetTargetFrameRate() const { return static_cast<float>(m_TargetFrameRate); }
        float  GetFixedDeltaTime() const { return static_cast<float>(m_FixedDeltaTime); }
        float  GetFrameRate() const { return 1.f / static_cast<float>(m_DeltaTime); }
        float  GetGameTime() const { return static_cast<float>(m_GameTime); }
        
        void SetTargetFrameRate(const float frameRate)
        {
            m_TargetFrameRate = static_cast<double>(frameRate);
            m_FixedDeltaTime = 1 / m_TargetFrameRate;
        }

    private:
        double m_TargetFrameRate = 60;
        double m_GameTime = 0;
        double m_Elapsed = 0;
        double m_DeltaTime = 0;
        double m_FixedDeltaTime = 0;
        LARGE_INTEGER m_LastTime{};
        LARGE_INTEGER m_Frec{};
    
        LARGE_INTEGER GetTime();
    };
}

