#pragma once

namespace Nit::Math
{
    constexpr float pi = 3.14159265358979323846f;

    float ToRadians(float degrees);
    Vector3 ToRadians(const Vector3& degrees);
    float ToDegrees(float radians);
    Vector3 ToDegrees(const Vector3& radians);

    inline float FloatMax() { return std::numeric_limits<float>::max(); }
    Vector3 ScreenToWorldPoint(const Matrix4& ProjectionViewMatrix, const Vector2& screenPoint, const Vector2& windowSize);


    float Atan2(float Y, float X);

    template<class T>
    T Sign(const T A)
    {
        return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
    }

    template<class T>
    T Abs(const T A)
    {
        return (A > (T)0) ? A : -A;
    }

    template<class T>
    T Min(const T A, const T B)
    {
        return (A < B) ? A : B;
    }

    template<class T>
    T Max(const T A, const T B)
    {
        return (A > B) ? A : B;
    }
}