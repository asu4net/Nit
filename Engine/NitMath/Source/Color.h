#pragma once

namespace Nit
{   
    struct Vector3;
    struct Vector4;

    struct Color
    {
        static const Color Black;
        static const Color Blue;
        static const Color LightBlue;
        static const Color Clear;
        static const Color Cyan;
        static const Color Gray;
        static const Color DarkGray;
        static const Color Green;
        static const Color LightGreen;
        static const Color Grey;
        static const Color DarkGrey;
        static const Color Magenta;
        static const Color Red;
        static const Color LightRed;
        static const Color White;
        static const Color Yellow;
        static const Color Orange;

        float r, g, b, a;

        Color();
        Color(float r, float g, float b, float a);
        Color(const Vector3& other, float a = 0);
        Color(const Vector4& other);

        Color operator+(const Color& other) const;
        void operator+=(const Color& other);
        Color operator-(const Color& other) const;
        void operator-=(const Color& other);
        Color operator*(float num) const;
        void operator*=(float num);
        Color operator/(float num) const;
        void operator/=(float num);
        bool operator<=(const Color& other) const;
        bool operator==(const Color& other) const;
        bool operator!=(const Color& other) const;
    };
}