#pragma once


#include <cmath>
#include "../core/core/types.h"

constexpr f64 RAIN_PI = 3.1415926535897932384626433832795;
constexpr f64 RAIN_HALFPI = 1.5707963267948966192313216916395;
constexpr f64 RAIN_TWOPI = 6.2831853071795864769252867665590;

namespace rain::math
{

    inline f32 to_rad(const f32& _angle)
    {
        return (_angle * f32(RAIN_PI)) / 180.f;
    }


    inline f32 to_degre(const f32& _angle)
    {
        return (_angle * 180.f) / f32(RAIN_PI);
    }

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
}