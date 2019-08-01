#pragma once

#include "glm.hpp"

namespace rain::core
{
    struct sphere
    {
        sphere()
            : sphere(0.0f, glm::vec3{})
        {}

        sphere(f32 _radius, glm::vec3 _offset)
            : radius(_radius)
            , offset(_offset)
        {}

        // offset from the center of the object - which must have a transform
        f32 radius;
        glm::vec3 offset;
    };
}