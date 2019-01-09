#pragma once


#include "core/core.h"

#define RAIN_PI     3.1415926535897932384626433832795
#define RAIN_HALFPI 1.5707963267948966192313216916395
#define RAIN_TWOPI  6.2831853071795864769252867665590

namespace rain::core::math
{

    inline f32 ToRad(const f32& _angle)
    {
        return (_angle * f32(RAIN_PI)) / 180.f;
    }


    inline f32 ToDeg(const f32& _angle)
    {
        return (_angle * 180.f) / f32(RAIN_PI);
    }
}