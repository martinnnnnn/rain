#pragma once

#include "math/numbers/math_basics.h"

namespace rain::math
{
    struct vec2
    {
        union
        {
            struct
            {
                f32 x;
                f32 y;
            };
            f32 data[2];
        };

        f32& operator[](u32 i) { return data[i]; }
        const f32& operator[](u32 i) const { return data[i]; }
    };

    vec2 operator+(const vec2& _a, const vec2& _b);
    vec2 operator-(const vec2& _a, const vec2& _b);
    vec2 operator*(const vec2& _a, const vec2& _b);
    vec2 operator*(const vec2& _a, f32 _b);
    vec2 operator/(const vec2& _a, const vec2& _b);
    vec2 operator/(const vec2& _a, f32 _b);
    bool operator==(const vec2& _a, const vec2& _b);
    bool operator!=(const vec2& _a, const vec2& _b);
    vec2& operator+=(vec2& _a, const vec2& _b);
    vec2& operator-=(vec2& _a, const vec2& _b);
    vec2& operator*=(vec2& _a, const vec2& _b);
    vec2& operator*=(vec2& _a, const f32 _b);
    vec2& operator/=(vec2& _a, const vec2& _b);
    vec2& operator/=(vec2& _a, const f32 _b);

    f32 dot(const vec2& _a, const vec2& _b);
    f32 magnitude(const vec2& _v);
    f32 magnitude_sq(const vec2& _v);
    f32 distance(const vec2& _a, const vec2& _b);
    f32 distance_sq(const vec2& _a, const vec2& _b);
    vec2 rotate(const vec2& _vec, f32 _deg);
    void normalize(vec2& _v);
    vec2 normalized(const vec2& _v);
    f32 angle(const vec2& _a, const vec2& _b);
    vec2 project(const vec2& _length, const vec2& _direction);
    vec2 perpendicular(const vec2& _length, const vec2& _direction);
    vec2 reflection(const vec2& _vec, const vec2& _normal);
}