#pragma once


#include "core/types.h"

#define RAIN_FLT_EPSILON 0.0005f

namespace rain::math
{
    bool is_equal(f32 _a, f32 _b, f32 _epsilon = RAIN_FLT_EPSILON)
    {
        return (fabs(_a - _b) <= _epsilon);
    }
}