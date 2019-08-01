#pragma once

#include "glm.hpp"

namespace rain::core
{
    struct ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };
}