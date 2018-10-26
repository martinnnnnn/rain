#pragma once


#include <glm/glm.hpp>

#include "core/types.h"

namespace rain
{
    struct RigidBody
    {
        glm::vec3 currentPosition;
        glm::vec3 previousPosition;

        glm::vec3 velocity;
        f32 angularVelocity;
    };
}