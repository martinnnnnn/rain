#pragma once

#include <glm/glm.hpp>

namespace rain::core::math
{
    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };
}