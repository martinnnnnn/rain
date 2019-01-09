#include "core/math/float.h"

#include <math.h>

namespace rain::core::math
{
    bool is_equal(f32 _a, f32 _b, f32 _epsilon)
    {
        return (fabs(_a - _b) <= _epsilon);
    }
}