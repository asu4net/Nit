#include "Vector4.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<Vector4>("Vector4")
        .constructor<>()
        .property("x", &Vector4::x)
        .property("y", &Vector4::y)
        .property("z", &Vector4::z)
        .property("w", &Vector4::w);
}

namespace Nit
{
    const Vector4 Vector4::Zero = { 0, 0, 0, 0 };

    Vector4::Vector4()
        : x(0)
        , y(0)
        , z(0)
        , w(0)
    {}

    Vector4::Vector4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {}

    Vector4::Vector4(const Vector3& other, float w)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , w(w)
    {}

    Vector4 Vector4::operator+(const Vector4& other) const
    {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    void Vector4::operator+=(const Vector4& other)
    {
        *this = *this + other;
    }

    Vector4 Vector4::operator-(const Vector4& other) const
    {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    void Vector4::operator-=(const Vector4& other)
    {
        *this = *this - other;
    }

    Vector4 Vector4::operator*(float num) const
    {
        return { x * num, y * num, z * num, w * num };
    }

    Vector4 Vector4::operator*(const Matrix4& other) const
    {
        Vector4 v;
        v.x = x * other.m[0][0] + y * other.m[1][0] + z * other.m[2][0] + w * other.m[3][0];
        v.y = x * other.m[0][1] + y * other.m[1][1] + z * other.m[2][1] + w * other.m[3][1];
        v.z = x * other.m[0][2] + y * other.m[1][2] + z * other.m[2][2] + w * other.m[3][2];
        v.w = x * other.m[0][3] + y * other.m[1][3] + z * other.m[2][3] + w * other.m[3][3];
        return v;
    }

    void Vector4::operator*=(float num)
    {
        *this = *this * num;
    }

    Vector4 Vector4::operator/(float num) const
    {
        return { x / num, y / num, z / num, w / num };
    }

    void Vector4::operator/=(float num)
    {
        *this = *this / num;
    }

    bool Vector4::operator<=(const Vector4& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
    }

    Vector4 Vector4::Abs() const
    {
        return { std::abs(x), std::abs(y), std::abs(z), std::abs(w) };
    }

    float Vector4::Magnitude() const
    {
        return std::sqrt(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2) + std::powf(w, 2));
    }

    Vector4 Vector4::Normalized() const
    {
        float mag = Magnitude();
        return { x / mag, y / mag, z / mag, w / mag };
    }

    void Vector4::Normalize()
    {
        *this = this->Normalized();
    }

    String Vector4::ToString() const
    {
        return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
    }

    float Vector4::Distance(const Vector4& a, const Vector4& b)
    {
        return std::sqrtf(std::powf(a.x - b.x, 2) + std::powf(a.y - b.y, 2) + std::powf(a.z - b.z, 2) + std::powf(a.w - b.w, 2));
    }
}