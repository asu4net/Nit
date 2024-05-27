#include "Math.h"

namespace Nit::Math
{
    inline static constexpr float DegreesToRadiansFactor = 0.0174533f;
    inline static constexpr float RadiansToDegreesFactor = 57.2958f;

    float ToRadians(float degrees)
    {
        return degrees * DegreesToRadiansFactor;
    }

    Vector3 ToRadians(const Vector3& degrees)
    {
        return degrees * DegreesToRadiansFactor;
    }

    float ToDegrees(float radians)
    {
        return radians * RadiansToDegreesFactor;
    }

    Vector3 ToDegrees(const Vector3& radians)
    {
        return radians * RadiansToDegreesFactor;
    }

    Vector3 ScreenToWorldPoint(const Matrix4& ProjectionViewMatrix, const Vector2& screenPoint, const Vector2& windowSize)
    {
        const float halfScreenWidth = windowSize.x / 2.f;
        const float halfScreenHeight = windowSize.y / 2.f;
        const Matrix4 inverseMv = ProjectionViewMatrix.GetInverse();
        const float nearPlaneX = (screenPoint.x - halfScreenWidth) / halfScreenWidth;
        const float nearPlaneY = -((screenPoint.y - halfScreenHeight) / halfScreenHeight);
        const Vector4 nearPlane = { nearPlaneX, nearPlaneY, -1, 1 };
        Vector4 nearResult = inverseMv * nearPlane;
        nearResult /= nearResult.w;
        return { nearResult.x, nearResult.y, 0 };
    }

    float Atan2(float Y, float X)
    {
        const float absX = Abs(X);
        const float absY = Abs(Y);
        const bool yAbsBigger = (absY > absX);
        float t0 = yAbsBigger ? absY : absX; // Max(absY, absX)
        float t1 = yAbsBigger ? absX : absY; // Min(absX, absY)

        if (t0 == 0.f)
            return 0.f;

        float t3 = t1 / t0;
        float t4 = t3 * t3;

        static const float c[7] = {
            +7.2128853633444123e-03f,
            -3.5059680836411644e-02f,
            +8.1675882859940430e-02f,
            -1.3374657325451267e-01f,
            +1.9856563505717162e-01f,
            -3.3324998579202170e-01f,
            +1.0f
        };

        t0 = c[0];
        t0 = t0 * t4 + c[1];
        t0 = t0 * t4 + c[2];
        t0 = t0 * t4 + c[3];
        t0 = t0 * t4 + c[4];
        t0 = t0 * t4 + c[5];
        t0 = t0 * t4 + c[6];
        t3 = t0 * t3;

        t3 = yAbsBigger ? (0.5f * pi) - t3 : t3;
        t3 = (X < 0.0f) ? pi - t3 : t3;
        t3 = (Y < 0.0f) ? -t3 : t3;

        return t3;
    }

}