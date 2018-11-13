#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace rain
{
    //struct Transform
    //{
    //    glm::vec3 position;
    //    glm::quat rotation;
    //    glm::vec3 scale;
    //};

    struct Transform
    {
        Transform()
            : position(glm::vec3(0.0f))
            , previousPosition(glm::vec3(0.0f))
            , orientation(glm::quat(glm::vec3(0)))
            , previousOrientation(glm::quat())
        {}

        glm::vec3 position;
        glm::vec3 previousPosition;

        glm::quat orientation;
        glm::quat previousOrientation;
    };
}