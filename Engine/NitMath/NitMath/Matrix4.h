#pragma once
#include "NitMath/Vector3.h"
#include "NitMath/Vector4.h"

namespace Nit
{
    struct CMatrix4 // COLUMN MAJOR
    {
        union
        {
            float m[4][4]; //i = column, j = row
            float n[16];
        };
        
        CMatrix4();

        void SetIdentity();
        void SetZero();
        CMatrix4 GetTransposed() const;
        void SetTransposed();
        CMatrix4 GetInverse() const;
        float GetDeterminant() const;
        void SetInverse();

        CVector3 GetTranslation() const;

        CMatrix4 operator*(const CMatrix4& other) const;
        CVector4 operator*(const CVector4& other) const;
        void operator*=(const CMatrix4& other);
        bool operator==(const CMatrix4& other) const;
        bool operator!=(const CMatrix4& other) const;

        static CMatrix4 GetIdentity();
        static CMatrix4 CreateTransform(const CVector3& position, const CVector3& rotation = CVector3::Zero, const CVector3& scale = CVector3::One);
        static CMatrix4 Translate(const CMatrix4& matrix, const CVector3& translation);
        static CMatrix4 RotateX(const CMatrix4& matrix, float x);
        static CMatrix4 RotateY(const CMatrix4& matrix, float y);
        static CMatrix4 RotateZ(const CMatrix4& matrix, float z);
        static CMatrix4 Rotate(const CMatrix4& matrix, const CVector3& rotation);
        static CMatrix4 Scale(const CMatrix4& matrix, const CVector3& scale);
        static CMatrix4 OrthoProjection(const float aspectRatio, const float size, float nearPlane, float farPlane);
        static CMatrix4 OrthoProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane);
        static CMatrix4 ViewProjection(const CVector3& position, const CVector3& rotation);
    };
}