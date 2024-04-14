#pragma once

namespace Nit
{
    struct CVector3;
    struct CVector2
    {
        float x, y;

        static const CVector2 Zero;
        static const CVector2 One;
        static const CVector2 Right;
        static const CVector2 Left;
        static const CVector2 Up;
        static const CVector2 Down;

        CVector2();
        CVector2(float x, float y);
        CVector2(const CVector3& other);
        bool operator==(const CVector2& other) const;
        bool operator!=(const CVector2& other) const;
        CVector2 operator+(const CVector2& other) const;
        CVector2 operator-(const CVector2& other) const;
        CVector2 operator*(float number) const;
        CVector2 operator/(float number) const;
        void operator/=(float number);
        CVector2 Abs() const;
        float Magnitude() const;
        CVector2 Normalized() const;
        void Normalize();
        std::string ToString() const;
        
        static CVector2 RotateAround(CVector2 pivot, float angle, CVector2 point);
        static CVector2 Multiply(const CVector2& a, const CVector2& b);
        static CVector2 Divide(const CVector2& a, const CVector2& b);
        static float Dot(const CVector2& a, const CVector2& b);
        static float Distance(const CVector2& a, const CVector2& b);
        static float Angle(const CVector2& a, const CVector2& b);
    };
}
