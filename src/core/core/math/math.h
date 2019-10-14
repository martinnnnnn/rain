#pragma once


#include "transform.h"
#include "sphere.h"
#include "plane.h"
#include "ray.h"
#include "quick_hull.h"
#include "simplex_noise.h"

namespace rain::core
{
    inline bool get_quadratic_roots(const f32 a, const f32 b, const f32 c, f32& r1, f32& r2)
    {
        if ((a >= 0) && (b >= 0) && (c >= 0))
        {
            const f32 q = (b * b) - (4 * a * c);
            if (q >= 0.0f)
            {
                const f32 sq = sqrt(q);
                const f32 d = 1.0f / (2.0f * a);
                r1 = (-b + sq) * d;
                r2 = (-b - sq) * d;
                return true;
            }
        }
        return false;
    }

    inline bool is_inside_boundary(i32 x, i32 y, i32 z, i32 xmax, i32 ymax, i32 zmax, i32 xmin = 0, i32 ymin = 0, i32 zmin = 0)
    {
        return (x < xmax && y < ymax && z < zmax && x >= xmin && y >= ymin && z >= zmin);
    }

    inline i32 round(f32 value)
    {
        return i32(value + 0.5f);
    }
}