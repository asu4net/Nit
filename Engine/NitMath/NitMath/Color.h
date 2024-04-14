#pragma once

namespace Nit
{   
    struct CVector3;
    struct CVector4;

    struct CColor
    {
        static const CColor Black;
        static const CColor Blue;
        static const CColor LightBlue;
        static const CColor Clear;
        static const CColor Cyan;
        static const CColor Gray;
        static const CColor DarkGray;
        static const CColor Green;
        static const CColor LightGreen;
        static const CColor Grey;
        static const CColor DarkGrey;
        static const CColor Magenta;
        static const CColor Red;
        static const CColor LightRed;
        static const CColor White;
        static const CColor Yellow;
        static const CColor Orange;

        float r, g, b, a;

        CColor();
        CColor(float r, float g, float b, float a);
        CColor(const CVector3& other, float a = 0);
        CColor(const CVector4& other);

        CColor operator+(const CColor& other) const;
        void operator+=(const CColor& other);
        CColor operator-(const CColor& other) const;
        void operator-=(const CColor& other);
        CColor operator*(float num) const;
        void operator*=(float num);
        CColor operator/(float num) const;
        void operator/=(float num);
        bool operator<=(const CColor& other) const;
        bool operator==(const CColor& other) const;
        bool operator!=(const CColor& other) const;
    };
}