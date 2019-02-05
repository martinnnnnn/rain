#pragma once

#include "vec3.h"

namespace rain::math
{
    struct sphere
    {
        sphere()
            : sphere(0.0f, vec3{})
        {}

        sphere(float _radius, vec3 _offset)
            : radius(_radius)
            , offset(_offset)
        {}

        // offset from the center of the object - which must have a transform
        f32 radius;
        vec3 offset;
    };
}