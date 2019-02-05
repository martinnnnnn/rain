#pragma once

#include "vec3.h"

namespace rain::math
{
    struct ray
    {
        vec3 origin;
        vec3 direction;
    };
}