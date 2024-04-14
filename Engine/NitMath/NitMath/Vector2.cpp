#include "Vector2.h"
#include "Vector3.h"
#include "NitMath.h"

namespace Nit
{
    const CVector2 CVector2::Zero = { 0, 0 };
    const CVector2 CVector2::One = { 1, 1 };
    const CVector2 CVector2::Right = { 1, 0 };
    const CVector2 CVector2::Left = { -1, 0 };
    const CVector2 CVector2::Up = { 0, 1 };
    const CVector2 CVector2::Down = { 0, -1 };

    CVector2::CVector2()
        : x(0)
        , y(0)
    {}

    CVector2::CVector2(float x, float y)
        : x(x)
        , y(y)
    {}

    CVector2::CVector2(const CVector3& other)
        : x(other.x)
        , y(other.y)
    {}

    bool CVector2::operator==(const CVector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool CVector2::operator!=(const CVector2& other) const
    {
        return x != other.x || y != other.y;
    }

    CVector2 CVector2::operator+(const CVector2& other) const
    {
        return { x + other.x, y + other.y };
    }

    CVector2 CVector2::operator-(const CVector2& other) const
    {
        return { x - other.x, y - other.y };
    }

    CVector2 CVector2::operator*(float number) const
    {
        return { x * number, y * number };
    }

    CVector2 CVector2::operator/(float number) const
    {
        return { x / number, y / number };
    }

    void CVector2::operator/=(float number)
    {
        x /= number;
        y /= number;
    }

    CVector2 CVector2::Abs() const
    {
        return { std::abs(x), std::abs(y) };
    }

    float CVector2::Magnitude() const
    {
        return std::sqrt(std::powf(x, 2) + std::powf(y, 2));
    }

    CVector2 CVector2::Normalized() const
    {
        float mag = Magnitude();
        return { x / mag, y / mag };
    }

    void CVector2::Normalize()
    {
        *this = this->Normalized();
    }

    std::string CVector2::ToString() const
    {
        return std::to_string(x) + ", " + std::to_string(y);
    }

    CVector2 CVector2::Multiply(const CVector2& a, const CVector2& b)
    {
        CVector2 r;
        r.x = a.x * b.x;
        r.y = a.y * b.y;
        return r;
    }

    CVector2 CVector2::Divide(const CVector2& a, const CVector2& b)
    {
        CVector2 r;
        r.x = a.x / b.x;
        r.y = a.y / b.y;
        return r;
    }

    CVector2 CVector2::RotateAround(CVector2 pivot, float angle, CVector2 point)
    {
        float s = sin(Math::ToRadians(angle));
        float c = cos(Math::ToRadians(angle));

        // translate point back to origin:
        point.x -= pivot.x;
        point.y -= pivot.y;

        // rotate point
        float xnew = point.x * c - point.y * s;
        float ynew = point.x * s + point.y * c;

        // translate point back:
        point.x = xnew + pivot.x;
        point.y = ynew + pivot.y;
        return point;
    }
    
    float CVector2::Dot(const CVector2& a, const CVector2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    float CVector2::Distance(const CVector2& a, const CVector2& b)
    {
        return std::sqrtf(std::powf(a.x - b.x, 2) + std::powf(a.y - b.y, 2));
    }

    float CVector2::Angle(const CVector2& a, const CVector2& b)
    {
        float cosAngle = Dot(a, b) / (a.Magnitude() * b.Magnitude());
        return std::acosf(cosAngle);
    }
}