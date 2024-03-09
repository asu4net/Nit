#pragma once

namespace Nit
{
    class Time
    {
    public:
        static double MaxDeltaTime;
        static double FixedDeltaTime;

        static bool IsInitialized();
        static float GetDeltaTime();
        static float GetApplicationTime();
        static uint32_t GetFrameRate();
        static uint32_t GetApplicationFrames();
        static uint32_t GetFixedUpdateCalls();

        static void Init();
        static void CalculateTimeStep();

    private:
        inline static bool m_IsInitalized = false;
        
        inline static double   m_DeltaTime;
        inline static double   m_ApplicationTime;
        inline static uint32_t m_ApplicationFrames;
        inline static uint32_t m_FixedUpdateCalls;
        inline static double   m_FixedUpdateTimer;
        inline static double   m_LastTime;
    };
}