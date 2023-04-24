#pragma once
#include <glm/glm.hpp>
#include "glm/gtx/quaternion.hpp"

namespace Math
{
    inline glm::vec3 LookAt(const glm::quat& rot, const glm::vec3& axis)
    {
        const glm::mat4 matRot = glm::toMat4(rot);
        const glm::vec4 dir = glm::vec4(axis, 1);
        return {matRot * dir};
    }
}