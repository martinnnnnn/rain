#pragma once


#include "math/numbers/math_basics.h"
#include "math/numbers/float.h"

namespace rain::math
{

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

        inline f32& operator[](int i) { return data[i]; }
        inline const f32& operator[](int i) const { return data[i]; }
        inline vec3& operator=(const vec3& q)
        {
            this->x = q.x;
            this->y = q.y;
            this->z = q.z;
            return *this;
        }
    };

    vec3 operator+(const vec3& _a, const vec3& _b);
    vec3 operator-(const vec3& _a, const vec3& _b);
    vec3 operator*(const vec3& _a, const vec3& _b);
    vec3 operator*(const vec3& _a, f32 _b);
    vec3 operator*(const f32 _b, const vec3& _a);
    vec3 operator/(const vec3& _a, const vec3& _b);
    vec3 operator/(const vec3& _a, f32 _b);
    bool operator==(const vec3& _a, const vec3& _b);
    bool operator!=(const vec3& _a, const vec3& _b);
    vec3& operator+=(vec3& _a, const vec3& _b);
    vec3& operator-=(vec3& _a, const vec3& _b);
    vec3& operator*=(vec3& _a, const vec3& _b);
    vec3& operator*=(vec3& _a, const f32 _b);
    vec3& operator/=(vec3& _a, const vec3& _b);
    vec3& operator/=(vec3& _a, const f32 _b);

    inline vec3 operator-(const vec3& v)
    {
        return vec3{ -v.x, -v.y, -v.z };
    }

    f32 dot(const vec3& _a, const vec3& _b);
    f32 magnitude(const vec3& _v);
    f32 magnitude_sq(const vec3& _v);
    f32 distance(const vec3& _a, const vec3& _b);
    f32 distance_sq(const vec3& _a, const vec3& _b);
    void normalize(vec3& _v);
    vec3 normalized(const vec3& _v);
    vec3 cross(const vec3& _a, const vec3& _b);
    f32 angle(const vec3& _a, const vec3& _b);
    vec3 project(const vec3& _length, const vec3& _direction);
    vec3 perpendicular(const vec3& _length, const vec3& _direction);
    vec3 reflection(const vec3& _vec, const vec3& _normal);

    inline vec3 cos(const vec3& v)
    {
        return vec3
        {
            cos(v.x),
            cos(v.y),
            cos(v.z)
        };
    }

    inline vec3 sin(const vec3& v)
    {
        return vec3
        {
            sin(v.x),
            sin(v.y),
            sin(v.z)
        };
    }
}