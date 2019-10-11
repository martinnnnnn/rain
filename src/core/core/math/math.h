#pragma once


#include "transform.h"
#include "sphere.h"
#include "plane.h"
#include "ray.h"
#include "quick_hull.h"
#include "simplex_noise.h"

namespace rain::core
{
    inline bool get_quadratic_roots(const f32 _a, const f32 _b, const f32 _c, f32& _r1, f32& _r2)
    {
        if ((_a >= 0) && (_b >= 0) && (_c >= 0))
        {
            const f32 q = (_b * _b) - (4 * _a * _c);
            if (q >= 0.0f)
            {
                const f32 sq = sqrt(q);
                const f32 d = 1.0f / (2.0f * _a);
                _r1 = (-_b + sq) * d;
                _r2 = (-_b - sq) * d;
                return true;
            }
        }
        return false;
    }

    inline bool is_inside_boundary(i32 x, i32 y, i32 z, i32 xmax, i32 ymax, i32 zmax, i32 xmin = 0, i32 ymin = 0, i32 zmin = 0)
    {
        return (x < xmax && y < ymax && z < zmax && x >= xmin && y >= ymin && z >= zmin);
    }
}