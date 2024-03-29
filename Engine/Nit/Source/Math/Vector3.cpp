#include "Vector3.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<Vector3>("Vector3")
        .constructor<>()
        .property("x", &Vector3::x)
        .property("y", &Vector3::y)
        .property("z", &Vector3::z);
}

namespace Nit
{
    const Vector3 Vector3::Zero = { 0, 0, 0 };
    const Vector3 Vector3::One = { 1, 1, 1 };
    const Vector3 Vector3::Right = { 1, 0, 0 };
    const Vector3 Vector3::Left = { -1, 0, 0 };
    const Vector3 Vector3::Up = { 0, 1, 0 };
    const Vector3 Vector3::Down = { 0, -1, 0 };
    const Vector3 Vector3::Back = { 0, 0, -1 };
    const Vector3 Vector3::Forward = { 0, 0, 1 };

    Vector3::Vector3()
        : x(0)
        , y(0)
        , z(0)
    {}

    Vector3::Vector3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {}

    Vector3::Vector3(const Vector2& other, float z)
        : x(other.x)
        , y(other.y)
        , z(z)
    {}

    Vector3::Vector3(const Vector4& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
    {}

    Vector3 Vector3::operator+(const Vector3& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    void Vector3::operator+=(const Vector3& other)
    {
        *this = *this + other;
    }

    Vector3 Vector3::operator-(const Vector3& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    void Vector3::operator-=(const Vector3& other)
    {
        *this = *this - other;
    }

    Vector3 Vector3::operator*(float num) const
    {
        return { x * num, y * num, z * num };
    }

    void Vector3::operator*=(float num)
    {
        *this = *this * num;
    }

    Vector3 Vector3::operator/(float num) const
    {
        return { x / num, y / num, z / num };
    }

    void Vector3::operator/=(float num)
    {
        *this = *this / num;
    }

    bool Vector3::operator<=(const Vector3& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool Vector3::operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool Vector3::operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    /*Vector3 Vector3::LookAt(const glm::quat& rot, const Vector3& axis)
    {
        const glm::mat4 matRot = glm::toMat4(rot);
        const glm::vec4 dir = glm::vec4(axis.ToGlm(), 1);
        return { matRot * dir };
    }*/

    Vector3 Vector3::Abs() const
    {
        return { std::abs(x), std::abs(y), std::abs(z) };
    }

    float Vector3::Magnitude() const
    {
        return std::sqrt(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2));
    }

    Vector3 Vector3::Normalized() const
    {
        float mag = Magnitude();
        return { x / mag, y / mag, z / mag };
    }

    void Vector3::Normalize()
    {
        *this = this->Normalized();
    }

    String Vector3::ToString() const
    {
        return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
    }

    Vector3 Vector3::Multiply(const Vector3& a, const Vector3& b)
    {
        Vector3 r;
        r.x = a.x * b.x;
        r.y = a.y * b.y;
        r.z = a.z * b.z;
        return r;
    }

    Vector3 Vector3::Divide(const Vector3& a, const Vector3& b)
    {
        Vector3 r;
        r.x = a.x / b.x;
        r.y = a.y / b.y;
        r.z = a.z / b.z;
        return r;
    }

    float Vector3::Dot(const Vector3& a, const Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float Vector3::Distance(const Vector3& a, const Vector3& b)
    {
        return std::sqrtf(std::powf(a.x - b.x, 2) + std::powf(a.y - b.y, 2) + std::powf(a.z - b.z, 2));
    }

    Vector3 Vector3::LookAt(const Vector3& rotation, const Vector3& axis)
    {
        Matrix4 rotationMatrix;
        rotationMatrix = Matrix4::Rotate(rotationMatrix, Math::ToRadians(rotation));
        return rotationMatrix * axis;
    }
}
