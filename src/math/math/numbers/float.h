#pragma once

#include "math/numbers/math_basics.h"


constexpr f32 RAIN_FLT_EPSILON = 0.0005f;

namespace rain::math
{
    bool is_equal(f32 _a, f32 _b, f32 _epsilon = RAIN_FLT_EPSILON);
}