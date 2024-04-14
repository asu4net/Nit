#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace Nit
{
    const CVector4 CVector4::Zero = { 0, 0, 0, 0 };

    CVector4::CVector4()
        : x(0)
        , y(0)
        , z(0)
        , w(0)
    {}

    CVector4::CVector4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {}

    CVector4::CVector4(const CVector3& other, float w)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , w(w)
    {}

    CVector4 CVector4::operator+(const CVector4& other) const
    {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    void CVector4::operator+=(const CVector4& other)
    {
        *this = *this + other;
    }

    CVector4 CVector4::operator-(const CVector4& other) const
    {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    void CVector4::operator-=(const CVector4& other)
    {
        *this = *this - other;
    }

    CVector4 CVector4::operator*(float num) const
    {
        return { x * num, y * num, z * num, w * num };
    }

    CVector4 CVector4::operator*(const CMatrix4& other) const
    {
        CVector4 v;
        v.x = x * other.m[0][0] + y * other.m[1][0] + z * other.m[2][0] + w * other.m[3][0];
        v.y = x * other.m[0][1] + y * other.m[1][1] + z * other.m[2][1] + w * other.m[3][1];
        v.z = x * other.m[0][2] + y * other.m[1][2] + z * other.m[2][2] + w * other.m[3][2];
        v.w = x * other.m[0][3] + y * other.m[1][3] + z * other.m[2][3] + w * other.m[3][3];
        return v;
    }

    void CVector4::operator*=(float num)
    {
        *this = *this * num;
    }

    CVector4 CVector4::operator/(float num) const
    {
        return { x / num, y / num, z / num, w / num };
    }

    void CVector4::operator/=(float num)
    {
        *this = *this / num;
    }

    bool CVector4::operator<=(const CVector4& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
    }

    CVector4 CVector4::Abs() const
    {
        return { std::abs(x), std::abs(y), std::abs(z), std::abs(w) };
    }

    float CVector4::Magnitude() const
    {
        return std::sqrt(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2) + std::powf(w, 2));
    }

    CVector4 CVector4::Normalized() const
    {
        float mag = Magnitude();
        return { x / mag, y / mag, z / mag, w / mag };
    }

    void CVector4::Normalize()
    {
        *this = this->Normalized();
    }

    std::string CVector4::ToString() const
    {
        return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
    }

    float CVector4::Distance(const CVector4& a, const CVector4& b)
    {
        return std::sqrtf(std::powf(a.x - b.x, 2) + std::powf(a.y - b.y, 2) + std::powf(a.z - b.z, 2) + std::powf(a.w - b.w, 2));
    }
}