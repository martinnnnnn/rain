#pragma once


#include "core/core.h"

namespace rain::engine::math
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

        f32& operator[](int i) { return data[i]; }

        inline vec2() : x(0.0f), y(0.0f) {}
        inline vec2(f32 _x, f32 _y) : x(_x), y(_y) {}
    };

    struct vec3
    {
        union
        {
            struct
            {
                f32 x;
                f32 y;
                f32 z;
            };
            f32 data[3];
        };

        f32& operator[](int i) { return data[i]; }

        inline vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        inline vec3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}
    };

    vec2 operator+(const vec2& _a, const vec2& _b);
    vec3 operator+(const vec3& _a, const vec3& _b);

    vec2 operator-(const vec2& _a, const vec2& _b);
    vec3 operator-(const vec3& _a, const vec3& _b);

    vec2 operator*(const vec2& _a, const vec2& _b);
    vec3 operator*(const vec3& _a, const vec3& _b);

    vec2 operator*(const vec2& _a, f32 _b);
    vec3 operator*(const vec3& _a, f32 _b);

    vec2 operator/(const vec2& _a, const vec2& _b);
    vec3 operator/(const vec3& _a, const vec3& _b);

    vec2 operator/(const vec2& _a, f32 _b);
    vec3 operator/(const vec3& _a, f32 _b);

    bool operator==(const vec2& _a, const vec2& _b);
    bool operator==(const vec3& _a, const vec3& _b);

    bool operator!=(const vec2& _a, const vec2& _b);
    bool operator!=(const vec3& _a, const vec3& _b);

    vec2& operator+=(vec2& _a, const vec2& _b);
    vec2& operator-=(vec2& _a, const vec2& _b);
    vec2& operator*=(vec2& _a, const vec2& _b);
    vec2& operator*=(vec2& _a, const f32 _b);
    vec2& operator/=(vec2& _a, const vec2& _b);
    vec2& operator/=(vec2& _a, const f32 _b);

    vec3& operator+=(vec3& _a, const vec3& _b);
    vec3& operator-=(vec3& _a, const vec3& _b);
    vec3& operator*=(vec3& _a, const vec3& _b);
    vec3& operator*=(vec3& _a, const f32 _b);
    vec3& operator/=(vec3& _a, const vec3& _b);
    vec3& operator/=(vec3& _a, const f32 _b);

    f32 dot(const vec2& _a, const vec2& _b);
    f32 dot(const vec3& _a, const vec3& _b);

    f32 magnitude(const vec2& _v);
    f32 magnitude(const vec3& _v);

    f32 magnitude_sq(const vec2& _v);
    f32 magnitude_sq(const vec3& _v);

    f32 distance(const vec2& _a, const vec2& _b);
    f32 distance(const vec3& _a, const vec3& _b);

    f32 distance_sq(const vec2& _a, const vec2& _b);
    f32 distance_sq(const vec3& _a, const vec3& _b);

    vec2 rotate(const vec2& _vec, f32 _deg);

    void normalize(vec2& _v);
    void normalize(vec3& _v);

    vec2 normalized(const vec2& _v);
    vec3 normalized(const vec3& _v);

    vec3 cross(const vec3& _a, const vec3& _b);

    f32 angle(const vec2& _a, const vec2& _b);
    f32 angle(const vec3& _a, const vec3& _b);

    vec2 project(const vec2& _length, const vec2& _direction);
    vec3 project(const vec3& _length, const vec3& _direction);

    vec2 perpendicular(const vec2& _length, const vec2& _direction);
    vec3 perpendicular(const vec3& _length, const vec3& _direction);

    vec2 reflection(const vec2& _vec, const vec2& _normal);
    vec3 reflection(const vec3& _vec, const vec3& _normal);
}