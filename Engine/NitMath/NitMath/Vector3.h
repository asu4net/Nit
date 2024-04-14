#pragma once

namespace Nit
{
    struct CVector2;
    struct CVector4;

    struct CVector3
    {
        float x, y, z;

        static const CVector3 Zero;
        static const CVector3 One;
        static const CVector3 Right;
        static const CVector3 Left;
        static const CVector3 Up;
        static const CVector3 Down;
        static const CVector3 Forward;
        static const CVector3 Back;

        CVector3();
        CVector3(float x, float y, float z);
        CVector3(const CVector2& other, float z = 0);
        CVector3(const CVector4& other);

        CVector3 operator+(const CVector3& other) const;
        void operator+=(const CVector3& other);
        CVector3 operator-(const CVector3& other) const;
        void operator-=(const CVector3& other);
        CVector3 operator*(float num) const;
        void operator*=(float num);
        CVector3 operator/(float num) const;
        void operator/=(float num);
        bool operator<=(const CVector3& other) const;
        bool operator==(const CVector3& other) const;
        bool operator!=(const CVector3& other) const;
        
        CVector3 Abs() const;
        float Magnitude() const;
        CVector3 Normalized() const;
        void Normalize();
        std::string ToString() const;

        static CVector3 Multiply(const CVector3& a, const CVector3& b);
        static CVector3 Divide(const CVector3& a, const CVector3& b);
        
        static float Dot(const CVector3& a, const CVector3& b);
        static float Distance(const CVector3& a, const CVector3& b);
        static CVector3 LookAt(const CVector3& rotation, const CVector3& axis);
    };
}
