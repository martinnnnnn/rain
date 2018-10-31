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
            : currentPosition(glm::vec3(0.0f))
            , previousPosition(glm::vec3(0.0f))
        {}

        glm::vec3 currentPosition;
        glm::vec3 previousPosition;
    };
}