#include "Camera.h"
#include "Window/Input/Input.h"

namespace Nit
{
    Camera::Camera(const Projection startProjection)
        : CurrentProjection(startProjection)
        , Size(3.f)
        , Fov(85.f)
        , NearPlane(0.1f)
        , FarPlane(1000.f)
        , Position(VecForward * -2.f)
        , Rotation(MatIdentity)
        , AspectRatio(1280.f / 720.f)
        , m_ProjectionViewMatrix(MatIdentity)
    {
    }

    void Camera::UpdateMatrix()
    {
        mat4 viewMatrix = MatIdentity;
        CalculateView(viewMatrix);

        mat4 projectionMatrix = MatIdentity;
        switch (CurrentProjection)
        {
        case Projection::Perspective: CalculatePerspectiveProjection(projectionMatrix); break;
        case Projection::Orthographic: CalculateOrthographicProjection(projectionMatrix); break;
        case Projection::None: projectionMatrix = MatIdentity; break;
        }

        m_ProjectionViewMatrix = projectionMatrix * viewMatrix;
    }

    void Camera::CalculateView(mat4& viewMatrix)
    {
        vec3 tweakedPosition = Position;
        tweakedPosition.z *= -1;
        viewMatrix = glm::translate(mat4(1.0f), tweakedPosition) * glm::toMat4(Rotation);
        viewMatrix = glm::inverse(viewMatrix);
    }

    void Camera::CalculatePerspectiveProjection(mat4& projectionMatrix)
    {
        if (isnan(AspectRatio)) return;
        projectionMatrix = glm::perspective(glm::radians(Fov), AspectRatio,
            NearPlane, FarPlane);
    }

    void Camera::CalculateOrthographicProjection(mat4& projectionMatrix)
    {
        if (isnan(AspectRatio)) return;
        const float right = AspectRatio * Size; //update aspect ratio
        const float left = -right;
        projectionMatrix = glm::ortho(left, right, -Size, Size, NearPlane, FarPlane);
    }
}