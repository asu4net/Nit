#pragma once

namespace Nit
{
    struct Vector2;
    struct Vector4;

    struct Vector3
    {
        float x, y, z;

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Right;
        static const Vector3 Left;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Forward;
        static const Vector3 Back;

        Vector3();
        Vector3(float x, float y, float z);
        Vector3(const Vector2& other, float z = 0);
        Vector3(const Vector4& other);

        Vector3 operator+(const Vector3& other) const;
        void operator+=(const Vector3& other);
        Vector3 operator-(const Vector3& other) const;
        void operator-=(const Vector3& other);
        Vector3 operator*(float num) const;
        void operator*=(float num);
        Vector3 operator/(float num) const;
        void operator/=(float num);
        bool operator<=(const Vector3& other) const;
        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;
        
        Vector3 Abs() const;
        float Magnitude() const;
        Vector3 Normalized() const;
        void Normalize();
        std::string ToString() const;

        static Vector3 Multiply(const Vector3& a, const Vector3& b);
        static Vector3 Divide(const Vector3& a, const Vector3& b);
        
        static float Dot(const Vector3& a, const Vector3& b);
        static float Distance(const Vector3& a, const Vector3& b);
        static Vector3 LookAt(const Vector3& rotation, const Vector3& axis);
    };
}
