#pragma once

#include "vec3.h"

namespace rain::math
{
    struct Ray
    {
        vec3 origin;
        vec3 direction;
    };
}