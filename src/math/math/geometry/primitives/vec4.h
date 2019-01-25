#pragma once


#include "math/numbers/math_basics.h"

namespace rain::math
{
    struct vec4
    {
        union
        {
            struct
            {
                f32 x;
                f32 y;
                f32 z;
                f32 w;
            };
            f32 data[4];
        };

        f32& operator[](u32 i) { return data[i]; }
        const f32& operator[](u32 i) const { return data[i]; }
    };

    vec4 operator+(const vec4& _a, const vec4& _b);
    vec4 operator-(const vec4& _a, const vec4& _b);
    vec4 operator*(const vec4& _a, const vec4& _b);
    vec4 operator*(const vec4& _a, f32 _b);
    vec4 operator/(const vec4& _a, const vec4& _b);
    vec4 operator/(const vec4& _a, f32 _b);
    bool operator==(const vec4& _a, const vec4& _b);
    bool operator!=(const vec4& _a, const vec4& _b);
    vec4& operator+=(vec4& _a, const vec4& _b);
    vec4& operator-=(vec4& _a, const vec4& _b);
    vec4& operator*=(vec4& _a, const vec4& _b);
    vec4& operator*=(vec4& _a, const f32 _b);
    vec4& operator/=(vec4& _a, const vec4& _b);
    vec4& operator/=(vec4& _a, const f32 _b);

    f32 dot(const vec4& _a, const vec4& _b);
    f32 magnitude(const vec4& _v);
    f32 magnitude_sq(const vec4& _v);
    f32 distance(const vec4& _a, const vec4& _b);
    f32 distance_sq(const vec4& _a, const vec4& _b);
    void normalize(vec4& _v);
    vec4 normalized(const vec4& _v);
    vec4 cross(const vec4& _a, const vec4& _b);
    f32 angle(const vec4& _a, const vec4& _b);
    vec4 project(const vec4& _length, const vec4& _direction);
    vec4 perpendicular(const vec4& _length, const vec4& _direction);
    vec4 reflection(const vec4& _vec, const vec4& _normal);
}