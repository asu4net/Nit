#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Color.h"

namespace Nit::Math
{
    float   ToRadians(float degrees);
    Vector3 ToRadians(const Vector3& degrees);
    float   ToDegrees(float radians);
    Vector3 ToDegrees(const Vector3& radians);
    float   MapRange(float input, float inputMin, float inputMax, float outputMin, float outputMax);
    
    inline float FloatMax() { return std::numeric_limits<float>::max(); }
    inline int   IntMax()   { return std::numeric_limits<int>::max(); }

    Vector3 ScreenToWorldPoint(const Matrix4& ProjectionViewMatrix, const Vector2& screenPoint, const Vector2& windowSize);
}