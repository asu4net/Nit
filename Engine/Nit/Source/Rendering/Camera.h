#pragma once

namespace Nit
{
    enum class CameraProjection { None, Perspective, Orthographic };
    
    class Camera
    {
    public:
        CameraProjection Projection = CameraProjection::Perspective;
        float Size = 3.f;
        float Fov = 85.f;
        float NearPlane = 0.1f;
        float FarPlane = 1000.f;
        float AspectRatio = 1280.f / 720.f;
        
        Mat4 GetProjectionViewMat4() const { return m_ProjectionViewMat4; }
        
        Camera() = default;

        void CalculateProjectionViewMat4(const Vec3& position, const Quat& rotation);

        void CalculateProjection(Mat4& projectionMatrix) const;
        void CalculateView(Mat4& viewMatrix, const Vec3& position, const Quat& rotation) const;
        
    private:
        Mat4 m_ProjectionViewMat4 = MatIdentity;
        RTTR_ENABLE()
    };
}