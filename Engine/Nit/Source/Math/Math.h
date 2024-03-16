#pragma once

namespace Nit::Math
{
    float ToRadians(float degrees);
    Vector3 ToRadians(const Vector3& degrees);
    float ToDegrees(float radians);
    Vector3 ToDegrees(const Vector3& radians);

    inline float FloatMax() { return std::numeric_limits<float>::max(); }
    Vector3 ScreenToWorldPoint(const Matrix4& ProjectionViewMatrix, const Vector2& screenPoint, const Vector2& windowSize);
}