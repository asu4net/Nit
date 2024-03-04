#include "Color.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<Color>("Color")
        .constructor<>()
        .property("r", &Color::r)
        .property("g", &Color::g)
        .property("b", &Color::b)
        .property("a", &Color::a);
}

namespace Nit
{
    const Color Color::Black = { 0, 0, 0, 1 };
    const Color Color::Blue = { 0, 0, 1, 1 };
    const Color Color::LightBlue = { .3f, .3f, 1, 1 };
    const Color Color::Clear = { 0, 0, 0, 0 };
    const Color Color::Cyan = { 0, 1, 1, 1 };
    const Color Color::Gray = { .5f, .5f, .5f, 1 };
    const Color Color::DarkGray = { .2f, .2f, .2f, 1 };
    const Color Color::Green = { 0, 1, 0, 1 };
    const Color Color::LightGreen = { .3f, 1, .3f, 1 };
    const Color Color::Grey = { .5f, .5f, .5f, 1 };
    const Color Color::DarkGrey = { .2f, .2f, .2f, 1 };
    const Color Color::Magenta = { 1, 0, 1, 1 };
    const Color Color::Red = { 1, 0, 0, 1 };
    const Color Color::LightRed = { 1, .3f, .3f, 1 };
    const Color Color::White = { 1, 1, 1, 1 };
    const Color Color::Yellow = { 1, .92f, .016f, 1 };
    const Color Color::Orange = { .97f, .60f, .11f, 1 };

    Color::Color()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {}

    Color::Color(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}

    Color::Color(const Vector3& other, float a)
        : r(other.x)
        , g(other.y)
        , b(other.z)
        , a(a)
    {}

    Color::Color(const Vector4& other)
        : r(other.x)
        , g(other.y)
        , b(other.z)
        , a(other.w)
    {}

    Color Color::operator+(const Color& other) const
    {
        return { r + other.r, g + other.g, b + other.b, a + other.a };
    }

    void Color::operator+=(const Color& other)
    {
        *this = *this + other;
    }

    Color Color::operator-(const Color& other) const
    {
        return { r - other.r, g - other.g, b - other.b, a - other.a };
    }

    void Color::operator-=(const Color& other)
    {
        *this = *this - other;
    }

    Color Color::operator*(float num) const
    {
        return { r * num, g * num, b * num, a * num };
    }

    void Color::operator*=(float num)
    {
        *this = *this * num;
    }

    Color Color::operator/(float num) const
    {
        return { r / num, g / num, b / num, a / num };
    }

    void Color::operator/=(float num)
    {
        *this = *this / num;
    }

    bool Color::operator<=(const Color& other) const
    {
        return r <= other.r && g <= other.g && b <= other.b && a <= other.a;
    }

    bool Color::operator==(const Color& other) const
    {
        return r == other.r && g== other.g && b == other.b && a == other.a;
    }

    bool Color::operator!=(const Color& other) const
    {
        return r != other.r && g != other.g && b != other.b && a != other.a;
    }

    String Color::ToString() const
    {
        return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a);
    }
}