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
        .property("FarPlane", &Camera::FarPlane)
        .property("AspectRatio", &Camera::AspectRatio);
}

namespace Nit
{
    void Camera::CalculateProjectionViewMat4(const Vec3& position, const Quat& rotation)
    {
        using namespace glm;
        
        Vec3 tweakedPosition = position;
        tweakedPosition.z *= -1;
        Mat4 viewMatrix = translate(Mat4(1.0f), tweakedPosition) * toMat4(rotation);
        viewMatrix = inverse(viewMatrix);
            
        Mat4 projectionMatrix = MatIdentity;
            
        switch (Projection)
        {
        case CameraProjection::Perspective:
            {
                if (isnan(AspectRatio)) break;
                projectionMatrix = perspective(radians(Fov), AspectRatio,
                    NearPlane, FarPlane);   
            }
            break;
                    
        case CameraProjection::Orthographic:
            {
                if (isnan(AspectRatio)) break;
                const float right = AspectRatio * Size; //update aspect ratio
                const float left = -right;
                projectionMatrix = ortho(left, right, -Size, Size,
                    NearPlane, FarPlane);
            }
            break;
                    
        case CameraProjection::None: projectionMatrix = MatIdentity; break;
        }

        m_ProjectionViewMat4 = projectionMatrix * viewMatrix;
    }
}