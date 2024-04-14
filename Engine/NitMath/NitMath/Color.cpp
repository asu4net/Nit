#include "Color.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Nit
{
    const CColor CColor::Black = { 0, 0, 0, 1 };
    const CColor CColor::Blue = { 0, 0, 1, 1 };
    const CColor CColor::LightBlue = { .3f, .3f, 1, 1 };
    const CColor CColor::Clear = { 0, 0, 0, 0 };
    const CColor CColor::Cyan = { 0, 1, 1, 1 };
    const CColor CColor::Gray = { .5f, .5f, .5f, 1 };
    const CColor CColor::DarkGray = { .2f, .2f, .2f, 1 };
    const CColor CColor::Green = { 0, 1, 0, 1 };
    const CColor CColor::LightGreen = { .3f, 1, .3f, 1 };
    const CColor CColor::Grey = { .5f, .5f, .5f, 1 };
    const CColor CColor::DarkGrey = { .2f, .2f, .2f, 1 };
    const CColor CColor::Magenta = { 1, 0, 1, 1 };
    const CColor CColor::Red = { 1, 0, 0, 1 };
    const CColor CColor::LightRed = { 1, .3f, .3f, 1 };
    const CColor CColor::White = { 1, 1, 1, 1 };
    const CColor CColor::Yellow = { 1, .92f, .016f, 1 };
    const CColor CColor::Orange = { .97f, .60f, .11f, 1 };

    CColor::CColor()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {}

    CColor::CColor(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}

    CColor::CColor(const CVector3& other, float a)
        : r(other.x)
        , g(other.y)
        , b(other.z)
        , a(a)
    {}

    CColor::CColor(const CVector4& other)
        : r(other.x)
        , g(other.y)
        , b(other.z)
        , a(other.w)
    {}

    CColor CColor::operator+(const CColor& other) const
    {
        return { r + other.r, g + other.g, b + other.b, a + other.a };
    }

    void CColor::operator+=(const CColor& other)
    {
        *this = *this + other;
    }

    CColor CColor::operator-(const CColor& other) const
    {
        return { r - other.r, g - other.g, b - other.b, a - other.a };
    }

    void CColor::operator-=(const CColor& other)
    {
        *this = *this - other;
    }

    CColor CColor::operator*(float num) const
    {
        return { r * num, g * num, b * num, a * num };
    }

    void CColor::operator*=(float num)
    {
        *this = *this * num;
    }

    CColor CColor::operator/(float num) const
    {
        return { r / num, g / num, b / num, a / num };
    }

    void CColor::operator/=(float num)
    {
        *this = *this / num;
    }

    bool CColor::operator<=(const CColor& other) const
    {
        return r <= other.r && g <= other.g && b <= other.b && a <= other.a;
    }

    bool CColor::operator==(const CColor& other) const
    {
        return r == other.r && g== other.g && b == other.b && a == other.a;
    }

    bool CColor::operator!=(const CColor& other) const
    {
        return r != other.r && g != other.g && b != other.b && a != other.a;
    }
}