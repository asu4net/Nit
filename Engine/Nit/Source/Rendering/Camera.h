#pragma once

namespace Nit
{
    class Window;
    
    class Camera
    {
    public:
        enum class Projection { None, Perspective, Orthographic };
        
        Projection CurrentProjection;
        float Size;
        float Fov;
        float NearPlane;
        float FarPlane;
        vec3 Position;
        glm::quat Rotation;
        float AspectRatio;
        
        Camera(Projection startProjection = Projection::Perspective);
        virtual ~Camera() = default;
        
        mat4 ProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

        void UpdateMatrix();

        void CalculateView(mat4& viewMatrix);
        void CalculatePerspectiveProjection(mat4& projectionMatrix);
        void CalculateOrthographicProjection(mat4& projectionMatrix);
        
    private:
        mat4 m_ProjectionViewMatrix;
    };
}