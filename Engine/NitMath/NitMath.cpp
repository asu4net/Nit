#include "NitMath.h"

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

    float MapRange(float input, float inputMin, float inputMax, float outputMin, float outputMax)
    {
        return outputMin + (input - inputMin) * (outputMax - outputMin) / (inputMax - inputMin);
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
}