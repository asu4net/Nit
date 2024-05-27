#pragma once

namespace Nit
{
    class Matrix4;

    struct Vector4
    {
        float x, y, z, w;

        Vector4();
        Vector4(float x, float y, float z, float w);
        Vector4(const Vector3& other, float w = 0);

        static const Vector4 Zero;
        static const Vector4 One;

        Vector4 operator+(const Vector4& other) const;
        void operator+=(const Vector4& other);
        Vector4 operator-(const Vector4& other) const;
        void operator-=(const Vector4& other);
        Vector4 operator*(float num) const;
        Vector4 operator*(const Vector4& other) const;
        Vector4 operator*(const Matrix4& other) const;
        void operator*=(float num);
        Vector4 operator/(float num) const;
        void operator/=(float num);
        bool operator<=(const Vector4& other) const;
        
        Vector4 Abs() const;
        float Magnitude() const;
        Vector4 Normalized() const;
        void Normalize();
        String ToString() const;

        //static float Dot(const Vector3& a, const Vector3& b);

        static float Distance(const Vector4& a, const Vector4& b);

        RTTR_ENABLE_NO_VIRTUAL
    };
}