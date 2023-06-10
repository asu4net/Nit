#include "Camera.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::enumeration<CameraProjection>("CameraProjection")
    (
        value("None", CameraProjection::None),
        value("Perspective", CameraProjection::Perspective),
        value("Orthographic", CameraProjection::Orthographic)
    );
    
    registration::class_<Camera>("Camera")
        .constructor<>()
        .property("Projection", &Camera::Projection)
        .property("Size", &Camera::Size)
        .property("Fov", &Camera::Fov)
        .property("NearPlane", &Camera::NearPlane)
        .property("ScreenWidth", &Camera::ScreenWidth)
        .property("ScreenHeight", &Camera::ScreenHeight);
}

namespace Nit
{
    void Camera::CalculateProjectionViewMat4(const Vec3& position, const Quat& rotation)
    {
        Mat4 viewMatrix = MatIdentity;
        CalculateView(viewMatrix, position, Quat(rotation));
        Mat4 projectionMatrix = MatIdentity;
        CalculateProjection(projectionMatrix);
        m_ProjectionViewMat4 = projectionMatrix * viewMatrix;
    }

    void Camera::CalculateProjection(Mat4& projectionMatrix) const
    {
        using namespace glm;

        const float aspect = static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);
        
        if (isnan(aspect))
            return;  

        switch (Projection)
        {
        case CameraProjection::Perspective:
            {
                if (isnan(aspect)) break;
                projectionMatrix = perspective(radians(Fov), aspect,
                    NearPlane, FarPlane);   
            }
            break;
        
        case CameraProjection::Orthographic:
            {
                if (isnan(aspect)) break;
                const float right = aspect * Size; //update aspect ratio
                const float left = -right;
                projectionMatrix = ortho(left, right, -Size, Size,
                    NearPlane, FarPlane);
            }
            break;
                    
        case CameraProjection::None: projectionMatrix = MatIdentity; break;
        }
    }

    void Camera::CalculateView(Mat4& viewMatrix, const Vec3& position, const Quat& rotation) const
    {
        using namespace glm;
        
        Vec3 tweakedPosition = position;
        tweakedPosition.z *= -1;
        viewMatrix = translate(Mat4(1.0f), tweakedPosition) * toMat4(rotation);
        viewMatrix = inverse(viewMatrix);
    }
}
