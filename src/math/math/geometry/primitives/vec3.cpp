#include "vec3.h"

#include <cmath>
#include <cassert>

#include "math/numbers/float.h"

namespace rain::math
{

    vec3 operator+(const vec3& _a, const vec3& _b)
    {
        return vec3{ _a.x + _b.x, _a.y + _b.y, _a.z + _b.z };
    }

    vec3 operator-(const vec3& _a, const vec3& _b)
    {
        return vec3{ _a.x - _b.x, _a.y - _b.y, _a.z - _b.z };
    }

    vec3 operator*(const vec3& _a, const vec3& _b)
    {
        return vec3{ _a.x * _b.x, _a.y * _b.y, _a.z * _b.z };
    }


    vec3 operator*(const vec3& _a, f32 _b)
    {
        return vec3{ _a.x * _b, _a.y * _b, _a.z * _b };
    }

    vec3 operator*(const f32 _b, const vec3& _a)
    {
        return vec3{ _a.x * _b, _a.y * _b, _a.z * _b };
    }


    vec3 operator/(const vec3& _a, const vec3& _b)
    {
        return vec3{ _a.x / _b.x, _a.y / _b.y, _a.z / _b.z };
    }


    vec3 operator/(const vec3& _a, f32 _b)
    {
        return vec3{ _a.x / _b, _a.y / _b, _a.z / _b };
    }


    bool operator==(const vec3& _a, const vec3& _b)
    {
        return (is_equal(_a.x, _b.x) && is_equal(_a.y, _b.y) && is_equal(_a.z, _b.z));
    }

    bool operator!=(const vec3& _a, const vec3& _b)
    {
        return !(_a == _b);
    }

    vec3& operator+=(vec3& _a, const vec3& _b)
    {
        _a.x += _b.x;
        _a.y += _b.y;
        _a.z += _b.z;
        return _a;
    }


    vec3& operator*=(vec3& _a, const vec3& _b)
    {
        _a.x *= _b.x;
        _a.y *= _b.y;
        _a.z *= _b.z;
        return _a;
    }

    vec3& operator*=(vec3& _vec, const f32 _b)
    {
        _vec.x *= _b;
        _vec.y *= _b;
        _vec.z *= _b;
        return _vec;
    }


    vec3& operator/=(vec3& _a, const vec3& _b)
    {
        _a.x /= _b.x;
        _a.y /= _b.y;
        _a.z /= _b.z;
        return _a;
    }

    vec3& operator/=(vec3& _vec, const f32 _b)
    {
        _vec.x /= _b;
        _vec.y /= _b;
        _vec.z /= _b;
        return _vec;
    }

    vec3& operator-=(vec3& _a, const vec3& _b)
    {
        _a.x -= _b.x;
        _a.y -= _b.y;
        _a.z -= _b.z;
        return _a;
    }


    f32 dot(const vec3& _a, const vec3& _b)
    {
        return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
    }


    f32 magnitude(const vec3& _v)
    {
        return sqrtf(dot(_v, _v));
    }


    f32 magnitude_sq(const vec3& _v)
    {
        return dot(_v, _v);
    }


    f32 distance(const vec3& _a, const vec3& _b)
    {
        return magnitude(_a - _b);
    }


    f32 distance_sq(const vec3& _a, const vec3& _b)
    {
        return magnitude_sq(_a - _b);
    }


    void normalize(vec3& _v)
    {
        _v = _v * (1.0f / magnitude(_v));
    }


    vec3 normalized(const vec3& _v)
    {
        return _v * (1.0f / magnitude(_v));
    }

    vec3 cross(const vec3& _a, const vec3& _b)
    {
        return vec3
        {
            _a.y * _b.z - _a.z * _b.y,
            _a.z * _b.x - _a.x * _b.z,
            _a.x * _b.y - _a.y * _b.x
        };
    }


    f32 angle(const vec3& l, const vec3& r)
    {
        return acosf(dot(l, r) / sqrtf(magnitude_sq(l) * magnitude_sq(r)));
    }


    vec3 project(const vec3& _length, const vec3& _direction)
    {
        return _direction * (dot(_length, _direction) / magnitude_sq(_direction));
    }



    vec3 perpendicular(const vec3& _length, const vec3& _direction)
    {
        return _length - project(_length, _direction);
    }


    vec3 reflection(const vec3& _vec, const vec3& _normal)
    {
        return _vec - _normal * (dot(_vec, _normal) *  2.0f);
    }

}


