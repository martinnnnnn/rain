#pragma once


#include <cmath>

#include "math/numbers/math_basics.h"


constexpr f32 RAIN_FLT_EPSILON = 0.0005f;

namespace rain::math
{
    bool is_equal(f32 _a, f32 _b, f32 _epsilon = RAIN_FLT_EPSILON);

    inline f32 cos(const f32& f)
    {
        return std::cos(f);
    }

    inline f32 sin(const f32& f)
    {
        return std::sin(f);
    }

    inline f32 tan(const f32& f)
    {
        return std::tan(f);
    }
}