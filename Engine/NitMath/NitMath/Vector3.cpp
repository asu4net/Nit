#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "NitMath.h"

namespace Nit
{
    const CVector3 CVector3::Zero = { 0, 0, 0 };
    const CVector3 CVector3::One = { 1, 1, 1 };
    const CVector3 CVector3::Right = { 1, 0, 0 };
    const CVector3 CVector3::Left = { -1, 0, 0 };
    const CVector3 CVector3::Up = { 0, 1, 0 };
    const CVector3 CVector3::Down = { 0, -1, 0 };
    const CVector3 CVector3::Back = { 0, 0, -1 };
    const CVector3 CVector3::Forward = { 0, 0, 1 };

    CVector3::CVector3()
        : x(0)
        , y(0)
        , z(0)
    {}

    CVector3::CVector3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {}

    CVector3::CVector3(const CVector2& other, float z)
        : x(other.x)
        , y(other.y)
        , z(z)
    {}

    CVector3::CVector3(const CVector4& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
    {}

    CVector3 CVector3::operator+(const CVector3& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    void CVector3::operator+=(const CVector3& other)
    {
        *this = *this + other;
    }

    CVector3 CVector3::operator-(const CVector3& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    void CVector3::operator-=(const CVector3& other)
    {
        *this = *this - other;
    }

    CVector3 CVector3::operator*(float num) const
    {
        return { x * num, y * num, z * num };
    }

    void CVector3::operator*=(float num)
    {
        *this = *this * num;
    }

    CVector3 CVector3::operator/(float num) const
    {
        return { x / num, y / num, z / num };
    }

    void CVector3::operator/=(float num)
    {
        *this = *this / num;
    }

    bool CVector3::operator<=(const CVector3& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool CVector3::operator==(const CVector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool CVector3::operator!=(const CVector3& other) const
    {
        return !(*this == other);
    }

    CVector3 CVector3::Abs() const
    {
        return { std::abs(x), std::abs(y), std::abs(z) };
    }

    float CVector3::Magnitude() const
    {
        return std::sqrt(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2));
    }

    CVector3 CVector3::Normalized() const
    {
        float mag = Magnitude();
        return { x / mag, y / mag, z / mag };
    }

    void CVector3::Normalize()
    {
        *this = this->Normalized();
    }

    std::string CVector3::ToString() const
    {
        return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
    }

    CVector3 CVector3::Multiply(const CVector3& a, const CVector3& b)
    {
        CVector3 r;
        r.x = a.x * b.x;
        r.y = a.y * b.y;
        r.z = a.z * b.z;
        return r;
    }

    CVector3 CVector3::Divide(const CVector3& a, const CVector3& b)
    {
        CVector3 r;
        r.x = a.x / b.x;
        r.y = a.y / b.y;
        r.z = a.z / b.z;
        return r;
    }

    float CVector3::Dot(const CVector3& a, const CVector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float CVector3::Distance(const CVector3& a, const CVector3& b)
    {
        return std::sqrtf(std::powf(a.x - b.x, 2) + std::powf(a.y - b.y, 2) + std::powf(a.z - b.z, 2));
    }

    CVector3 CVector3::LookAt(const CVector3& rotation, const CVector3& axis)
    {
        CMatrix4 rotationMatrix;
        rotationMatrix = CMatrix4::Rotate(rotationMatrix, Math::ToRadians(rotation));
        return rotationMatrix * axis;
    }
}