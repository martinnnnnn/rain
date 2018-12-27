#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace rain::core::math
{
    struct Transform
    {
        Transform()
            : position(glm::vec3(0.0f))
            , lastPosition(glm::vec3(0.0f))
            , orientation(glm::quat(glm::vec3(0)))
            , lastOrientation(glm::quat())
            , scale(glm::vec3(1.0f))
            , lastScale(glm::vec3(1.0f))
        {}

        const glm::mat4 Transform::get() const;

        glm::vec3 position;
        glm::vec3 lastPosition;

        glm::quat orientation;
        glm::quat lastOrientation;

        glm::vec3 scale;
        glm::vec3 lastScale;
    };

}