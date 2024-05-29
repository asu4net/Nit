#include "Matrix4.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    
rttr::registration::class_<Matrix4>("Matrix4")
.constructor<>();
        //.property("n", &Matrix4::n);
}

namespace Nit
{
    Matrix4::Matrix4()
    {
        SetIdentity();
    }

    void Matrix4::SetIdentity()
    {
        memset(m, 0, sizeof(float) * 16);
        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
        m[3][3] = 1;
    }
    
    void Matrix4::SetZero()
    {
        memset(m, 0, sizeof(float) * 16);
    }

    Matrix4 Matrix4::GetTransposed() const
    {
        Matrix4 transposed;
        transposed.m[0][1] = m[1][0];
        transposed.m[0][2] = m[2][0];
        transposed.m[0][3] = m[3][0];
        transposed.m[1][0] = m[0][1];
        transposed.m[1][2] = m[2][1];
        transposed.m[1][3] = m[3][1];
        transposed.m[2][0] = m[0][2];
        transposed.m[2][1] = m[1][2];
        transposed.m[2][3] = m[3][2];
        transposed.m[3][0] = m[0][3];
        transposed.m[3][1] = m[1][3];
        transposed.m[3][2] = m[2][3];
        return transposed;
    }

    void Matrix4::SetTransposed()
    {
        *this = GetTransposed();
    }

    Matrix4 Matrix4::GetInverse() const
    {
        Matrix4 inverse;
        
        float det = this->GetDeterminant();

        if (0.0f == det) {
            NIT_LOG_WARN("Matrix has no determinant. Can not invert\n");
            return *this;
        }

        float invDet = 1.0f / det;

        inverse.n[0] = invDet * (n[9] * n[14] * n[7] - n[13] * n[10] * n[7] +
            n[13] * n[6] * n[11] - n[5] * n[14] * n[11] -
            n[9] * n[6] * n[15] + n[5] * n[10] * n[15]);
        inverse.n[1] = invDet * (n[13] * n[10] * n[3] - n[9] * n[14] * n[3] -
            n[13] * n[2] * n[11] + n[1] * n[14] * n[11] +
            n[9] * n[2] * n[15] - n[1] * n[10] * n[15]);
        inverse.n[2] = invDet * (n[5] * n[14] * n[3] - n[13] * n[6] * n[3] +
            n[13] * n[2] * n[7] - n[1] * n[14] * n[7] -
            n[5] * n[2] * n[15] + n[1] * n[6] * n[15]);
        inverse.n[3] = invDet * (n[9] * n[6] * n[3] - n[5] * n[10] * n[3] -
            n[9] * n[2] * n[7] + n[1] * n[10] * n[7] +
            n[5] * n[2] * n[11] - n[1] * n[6] * n[11]);
        inverse.n[4] = invDet * (n[12] * n[10] * n[7] - n[8] * n[14] * n[7] -
            n[12] * n[6] * n[11] + n[4] * n[14] * n[11] +
            n[8] * n[6] * n[15] - n[4] * n[10] * n[15]);
        inverse.n[5] = invDet * (n[8] * n[14] * n[3] - n[12] * n[10] * n[3] +
            n[12] * n[2] * n[11] - n[0] * n[14] * n[11] -
            n[8] * n[2] * n[15] + n[0] * n[10] * n[15]);
        inverse.n[6] = invDet * (n[12] * n[6] * n[3] - n[4] * n[14] * n[3] -
            n[12] * n[2] * n[7] + n[0] * n[14] * n[7] +
            n[4] * n[2] * n[15] - n[0] * n[6] * n[15]);
        inverse.n[7] = invDet * (n[4] * n[10] * n[3] - n[8] * n[6] * n[3] +
            n[8] * n[2] * n[7] - n[0] * n[10] * n[7] -
            n[4] * n[2] * n[11] + n[0] * n[6] * n[11]);
        inverse.n[8] = invDet * (n[8] * n[13] * n[7] - n[12] * n[9] * n[7] +
            n[12] * n[5] * n[11] - n[4] * n[13] * n[11] -
            n[8] * n[5] * n[15] + n[4] * n[9] * n[15]);
        inverse.n[9] = invDet * (n[12] * n[9] * n[3] - n[8] * n[13] * n[3] -
            n[12] * n[1] * n[11] + n[0] * n[13] * n[11] +
            n[8] * n[1] * n[15] - n[0] * n[9] * n[15]);
        inverse.n[10] = invDet * (n[4] * n[13] * n[3] - n[12] * n[5] * n[3] +
            n[12] * n[1] * n[7] - n[0] * n[13] * n[7] -
            n[4] * n[1] * n[15] + n[0] * n[5] * n[15]);
        inverse.n[11] = invDet * (n[8] * n[5] * n[3] - n[4] * n[9] * n[3] -
            n[8] * n[1] * n[7] + n[0] * n[9] * n[7] +
            n[4] * n[1] * n[11] - n[0] * n[5] * n[11]);
        inverse.n[12] = invDet * (n[12] * n[9] * n[6] - n[8] * n[13] * n[6] -
            n[12] * n[5] * n[10] + n[4] * n[13] * n[10] +
            n[8] * n[5] * n[14] - n[4] * n[9] * n[14]);
        inverse.n[13] = invDet * (n[8] * n[13] * n[2] - n[12] * n[9] * n[2] +
            n[12] * n[1] * n[10] - n[0] * n[13] * n[10] -
            n[8] * n[1] * n[14] + n[0] * n[9] * n[14]);
        inverse.n[14] = invDet * (n[12] * n[5] * n[2] - n[4] * n[13] * n[2] -
            n[12] * n[1] * n[6] + n[0] * n[13] * n[6] +
            n[4] * n[1] * n[14] - n[0] * n[5] * n[14]);
        inverse.n[15] = invDet * (n[4] * n[9] * n[2] - n[8] * n[5] * n[2] +
                n[8] * n[1] * n[6] - n[0] * n[9] * n[6] -
                n[4] * n[1] * n[10] + n[0] * n[5] * n[10]);

        return inverse;
    }

    float Matrix4::GetDeterminant() const
    {
        return n[12] * n[9] * n[6] * n[3] -
            n[8] * n[13] * n[6] * n[3] -
            n[12] * n[5] * n[10] * n[3] +
            n[4] * n[13] * n[10] * n[3] +
            n[8] * n[5] * n[14] * n[3] -
            n[4] * n[9] * n[14] * n[3] -
            n[12] * n[9] * n[2] * n[7] +
            n[8] * n[13] * n[2] * n[7] +
            n[12] * n[1] * n[10] * n[7] -
            n[0] * n[13] * n[10] * n[7] -
            n[8] * n[1] * n[14] * n[7] +
            n[0] * n[9] * n[14] * n[7] +
            n[12] * n[5] * n[2] * n[11] -
            n[4] * n[13] * n[2] * n[11] -
            n[12] * n[1] * n[6] * n[11] +
            n[0] * n[13] * n[6] * n[11] +
            n[4] * n[1] * n[14] * n[11] -
            n[0] * n[5] * n[14] * n[11] -
            n[8] * n[5] * n[2] * n[15] +
            n[4] * n[9] * n[2] * n[15] +
            n[8] * n[1] * n[6] * n[15] -
            n[0] * n[9] * n[6] * n[15] -
            n[4] * n[1] * n[10] * n[15] +
            n[0] * n[5] * n[10] * n[15];
    }

    void Matrix4::SetInverse()
    {
        *this = this->GetInverse();
    }

    Vector3 Matrix4::GetTranslation() const
    {
        return { m[0][3], m[1][3], m[2][3] };
    }

    Matrix4 Matrix4::operator*(const Matrix4& other) const
    {
        Matrix4 result;
        for (uint32_t i = 0; i < 4; i++)
        {
            for (uint32_t j = 0; j < 4; j++)
            {
                result.m[i][j] =
                    m[0][j] * other.m[i][0] +
                    m[1][j] * other.m[i][1] +
                    m[2][j] * other.m[i][2] +
                    m[3][j] * other.m[i][3];
            }
        }
        return result;
    }

    void Matrix4::operator*=(const Matrix4& other)
    {
        *this = operator*(other);
    }

    Vector4 Matrix4::operator*(const Vector4& other) const
    {
        Vector4 v;
        v.x = other.x * m[0][0] + other.y * m[1][0] + other.z * m[2][0] + other.w * m[3][0];
        v.y = other.x * m[0][1] + other.y * m[1][1] + other.z * m[2][1] + other.w * m[3][1];
        v.z = other.x * m[0][2] + other.y * m[1][2] + other.z * m[2][2] + other.w * m[3][2];
        v.w = other.x * m[0][3] + other.y * m[1][3] + other.z * m[2][3] + other.w * m[3][3];
        return v;
    }

    bool Matrix4::operator==(const Matrix4& other) const
    {
        bool isEqual = true;

        for (uint32_t i = 0; i < 4; i++)
        {
            for (uint32_t j = 0; j < 4; j++)
            {
                if (m[i][j] == other.m[i][j])
                {
                    continue;
                }
                isEqual = false;
                break;
            }
        }

        return isEqual;
    }

    bool Matrix4::operator!=(const Matrix4& other) const
    {
        return !(*this == other);
    }

    Matrix4 Matrix4::CreateTransform(const Vector3& position, const Vector3& rotation /*= Vector3::Zero*/, const Vector3& scale /*= Vector3::One*/)
    {
        Matrix4 transform = Matrix4::Translate(Matrix4(), position)
            * Matrix4::Rotate(Matrix4(), Math::ToRadians(rotation))
            * Matrix4::Scale(Matrix4(), scale);
        return transform;
    }

    Matrix4 Matrix4::Translate(const Matrix4& matrix, const Vector3& translation)
    {
        Matrix4 translateMatrix;
        translateMatrix.m[3][0] = translation.x;
        translateMatrix.m[3][1] = translation.y;
        translateMatrix.m[3][2] = translation.z;
        return matrix * translateMatrix;
    }

    Matrix4 Matrix4::RotateX(const Matrix4& matrix, float x)
    {
        Matrix4 rotateMatrix;
        rotateMatrix.n[5] =  cosf(x);
        rotateMatrix.n[9] = -sinf(x);
        rotateMatrix.n[6] =  sinf(x);
        rotateMatrix.n[10] = cosf(x);
        return matrix * rotateMatrix;
    }

    Matrix4 Matrix4::RotateY(const Matrix4& matrix, float y)
    {
        Matrix4 rotateMatrix;
        rotateMatrix.n[0] =  cosf(y);
        rotateMatrix.n[8] =  sinf(y);
        rotateMatrix.n[2] = -sinf(y);
        rotateMatrix.n[10] = cosf(y);
        return matrix * rotateMatrix;
    }

    Matrix4 Matrix4::RotateZ(const Matrix4& matrix, float z)
    {
        Matrix4 rotateMatrix;
        rotateMatrix.n[0] =  cosf(z);
        rotateMatrix.n[4] = -sinf(z);
        rotateMatrix.n[1] =  sinf(z);
        rotateMatrix.n[5] =  cosf(z);
        return matrix * rotateMatrix;
    }

    Matrix4 Matrix4::Rotate(const Matrix4& matrix, const Vector3& rotation)
    {
        return RotateZ(RotateY(RotateX(matrix, rotation.x), rotation.y), rotation.z);
    }

    Matrix4 Matrix4::Scale(const Matrix4& matrix, const Vector3& scale)
    {
        Matrix4 scaleMatrix;

        scaleMatrix.m[0][0] = scale.x;
        scaleMatrix.m[1][1] = scale.y;
        scaleMatrix.m[2][2] = scale.z;

        return matrix * scaleMatrix;
    }

    Matrix4 Matrix4::OrthoProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane)
    {
        Matrix4 mat;
        mat.m[0][0] = 2 / (right - left);
        mat.m[1][1] = 2 / (top - bottom);
        mat.m[2][2] = -2 / (farPlane - nearPlane);
        mat.m[3][0] = -(right + left) / (right - left);
        mat.m[3][1] = -(top + bottom) / (top - bottom);
        mat.m[3][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        return mat;
    }

    Matrix4 Matrix4::OrthoProjection(const float aspectRatio, const float size, float nearPlane, float farPlane)
    {
        const float right = aspectRatio * size;
        const float left = -right;
        return Matrix4::OrthoProjection(left, right, -size, size, nearPlane, farPlane);
    }

    Matrix4 Matrix4::ViewProjection(const Vector3& position, const Vector3& rotation)
    {
        Vector3 tweakedPosition = position;
        //tweakedPosition.z *= -1;
        Matrix4 viewMatrix = Matrix4::CreateTransform(position, rotation);
        return viewMatrix.GetInverse();
    }

    Matrix4 Matrix4::PerspectiveProjection(float aspectRatio, float fov, float nearPlane, float farPlane)
    {
        const float tanHalfFovy = tan(Math::ToRadians(fov) / 2.f);
        const float range = farPlane - nearPlane;

        Matrix4 m; // make sure bottom-right corner is zero
        m.SetZero();

        m.m[0][0] = 1.f / (aspectRatio * tanHalfFovy);
        m.m[1][1] = 1.f / (tanHalfFovy);
        m.m[2][2] = -(farPlane + nearPlane) / range;
        m.m[2][3] = -1.f;
        m.m[3][2] = -2.f * farPlane * nearPlane / range;
        
        return m;
    }
}