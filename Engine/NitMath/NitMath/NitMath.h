#pragma once

#include "NitMath/Vector2.h"
#include "NitMath/Vector3.h"
#include "NitMath/Vector4.h"
#include "NitMath/Color.h"
#include "NitMath/Matrix4.h"

namespace Nit::Math
{
    float   ToRadians(float degrees);
    CVector3 ToRadians(const CVector3& degrees);
    float   ToDegrees(float radians);
    CVector3 ToDegrees(const CVector3& radians);
    float   MapRange(float input, float inputMin, float inputMax, float outputMin, float outputMax);
    
    inline float FloatMax() { return std::numeric_limits<float>::max(); }
    inline int   IntMax()   { return std::numeric_limits<int>::max(); }
    
    CVector3 ScreenToWorldPoint(const CMatrix4& ProjectionViewMatrix, const CVector2& screenPoint, const CVector2& windowSize);
}