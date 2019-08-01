#pragma once


#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtx/quaternion.hpp"

namespace rain::core
{

    struct transform
    {
        glm::vec3 position;
        glm::vec3 lastPosition;

        glm::quat orientation;
        glm::quat lastOrientation;

        glm::vec3 scale;
        glm::vec3 lastScale;
    };

    inline glm::mat4 get_transform_matrix(const transform& t)
    {
        return translate(glm::mat4(1), t.position) * glm::toMat4(t.orientation) * glm::scale(glm::mat4(1), t.scale);
    }

}