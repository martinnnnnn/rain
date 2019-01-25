#include "vec2.h"

#include <cmath>
#include <cassert>

#include "math/numbers/float.h"

namespace rain::math
{

    vec2 operator+(const vec2& _a, const vec2& _b)
    {
        return vec2{ _a.x + _b.x, _a.y + _b.y };
    }

    vec2 operator-(const vec2& _a, const vec2& _b)
    {
        return vec2{ _a.x - _b.x, _a.y - _b.y };
    }

    vec2 operator*(const vec2& _a, const vec2& _b)
    {
        return vec2{ _a.x * _b.x, _a.y * _b.y };
    }

    vec2 operator*(const vec2& _a, f32 _b)
    {
        return vec2{ _a.x * _b, _a.y * _b };
    }


    vec2 operator/(const vec2& _a, const vec2& _b)
    {
        return vec2{ _a.x / _b.x, _a.y / _b.y };
    }



    vec2 operator/(const vec2& _a, f32 _b)
    {
        return vec2{ _a.x / _b, _a.y / _b };
    }


    bool operator==(const vec2& _a, const vec2& _b)
    {
        return (is_equal(_a.x, _b.x) && is_equal(_a.y, _b.y));
    }


    bool operator!=(const vec2& _a, const vec2& _b)
    {
        return !(_a == _b);
    }


    vec2& operator+=(vec2& _a, const vec2& _b)
    {
        _a.x += _b.x;
        _a.y += _b.y;
        return _a;
    }

    vec2& operator-=(vec2& _a, const vec2& _b)
    {
        _a.x -= _b.x;
        _a.y -= _b.y;
        return _a;
    }

    vec2& operator*=(vec2& _a, const vec2& _b)
    {
        _a.x *= _b.x;
        _a.y *= _b.y;
        return _a;
    }

    vec2& operator*=(vec2& _vec, const f32 _b)
    {
        _vec.x *= _b;
        _vec.y *= _b;
        return _vec;
    }

    vec2& operator/=(vec2& _a, const vec2& _b)
    {
        _a.x /= _b.x;
        _a.y /= _b.y;
        return _a;
    }

    vec2& operator/=(vec2& _vec, const f32 _b)
    {
        _vec.x /= _b;
        _vec.y /= _b;
        return _vec;
    }


    f32 dot(const vec2& _a, const vec2& _b)
    {
        return _a.x * _b.x + _a.y * _b.y;
    }

    f32 magnitude(const vec2& _v)
    {
        return sqrtf(dot(_v, _v));
    }


    f32 magnitude_sq(const vec2& _v)
    {
        return dot(_v, _v);
    }

    f32 distance(const vec2& _a, const vec2& _b)
    {
        return magnitude(_a - _b);
    }


    f32 distance_sq(const vec2& _a, const vec2& _b)
    {
        return magnitude_sq(_a - _b);
    }


    vec2 rotate(const vec2& _vec, f32 _deg)
    {
        const f32 rad = ToRad(_deg);
        f32 s = sinf(rad);
        f32 c = cosf(rad);

        return vec2
        {
            _vec.x * c - _vec.y * s,
            _vec.x * s + _vec.y * c
        };
    }

    void normalize(vec2& _v)
    {
        _v = _v * (1.0f / magnitude(_v));
    }


    vec2 normalized(const vec2& _v)
    {
        return _v * (1.0f / magnitude(_v));
    }



    f32 angle(const vec2& _a, const vec2& _b)
    {
        return acosf(dot(_a, _b) / sqrtf(magnitude_sq(_a) * magnitude_sq(_b)));
    }


    vec2 project(const vec2& _length, const vec2& _direction)
    {
        return _direction * (dot(_length, _direction) / magnitude_sq(_direction));
    }


    vec2 perpendicular(const vec2& _length, const vec2& _direction)
    {
        return _length - project(_length, _direction);
    }


    vec2 reflection(const vec2& _vec, const vec2& _normal)
    {
        return _vec - _normal * (dot(_vec, _normal) *  2.0f);
    }


}


