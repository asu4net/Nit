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
        glm::vec3 Position;
        glm::quat Rotation;
        float AspectRatio;
        
        Camera(Projection startProjection = Projection::Perspective);
        virtual ~Camera() = default;
        
        glm::mat4 ProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

        void UpdateMatrix();

        void CalculateView(glm::mat4& viewMatrix);
        void CalculatePerspectiveProjection(glm::mat4& projectionMatrix);
        void CalculateOrthographicProjection(glm::mat4& projectionMatrix);
        
    private:
        glm::mat4 m_ProjectionViewMatrix;
    };
}