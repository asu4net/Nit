#pragma once

namespace Nit
{
    struct Vector3;
    struct Vector2
    {
        float x, y;

        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 Right;
        static const Vector2 Left;
        static const Vector2 Up;
        static const Vector2 Down;

        Vector2();
        Vector2(float x, float y);
        Vector2(const Vector3& other);
        bool operator==(const Vector2& other) const;
        bool operator!=(const Vector2& other) const;
        Vector2 operator+(const Vector2& other) const;
        Vector2 operator-(const Vector2& other) const;
        Vector2 operator*(float num) const;
        Vector2 operator/(float num) const;
        void operator/=(float num);
        Vector2 Abs() const;
        float Magnitude() const;
        Vector2 Normalized() const;
        void Normalize();
        String ToString() const;
        
        static Vector2 RotateAround(Vector2 pivot, float angle, Vector2 point);
        static Vector2 Multiply(const Vector2& a, const Vector2& b);
        static float Dot(const Vector2& a, const Vector2& b);
        static float Distance(const Vector2& a, const Vector2& b);
        static float Angle(const Vector2& a, const Vector2& b);

        RTTR_ENABLE_NO_VIRTUAL
    };
}
