#pragma once

namespace Nit
{
    struct CMatrix4;
    struct CVector3;

    struct CVector4
    {
        float x, y, z, w;

        CVector4();
        CVector4(float x, float y, float z, float w);
        CVector4(const CVector3& other, float w = 0);

        static const CVector4 Zero;

        CVector4 operator+(const CVector4& other) const;
        void operator+=(const CVector4& other);
        CVector4 operator-(const CVector4& other) const;
        void operator-=(const CVector4& other);
        CVector4 operator*(float num) const;
        CVector4 operator*(const CMatrix4& other) const;
        void operator*=(float num);
        CVector4 operator/(float num) const;
        void operator/=(float num);
        bool operator<=(const CVector4& other) const;
        
        CVector4 Abs() const;
        float Magnitude() const;
        CVector4 Normalized() const;
        void Normalize();
        std::string ToString() const;
        static float Distance(const CVector4& a, const CVector4& b);
    };
}