#include "vec4.h"

#include <cmath>
#include <cassert>

#include "math/numbers/float.h"

namespace rain::math
{

    vec4 operator+(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return vec4{ _a.x + _b.x, _a.y + _b.y, _a.z + _b.z };
    }

    vec4 operator-(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return vec4{ _a.x - _b.x, _a.y - _b.y, _a.z - _b.z };
    }

    vec4 operator*(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return vec4{ _a.x * _b.x, _a.y * _b.y, _a.z * _b.z };
    }


    vec4 operator*(const vec4& v, f32 f)
    {
        return vec4{ v.x * f, v.y * f, v.z * f, v.w * f };
    }


    vec4 operator/(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return vec4{ _a.x / _b.x, _a.y / _b.y, _a.z / _b.z };
    }


    vec4 operator/(const vec4& _a, f32 _b)
    {
        assert(false && "not ported yet");
        return vec4{ _a.x / _b, _a.y / _b, _a.z / _b };
    }


    bool operator==(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return (is_equal(_a.x, _b.x) && is_equal(_a.y, _b.y) && is_equal(_a.z, _b.z));
    }

    bool operator!=(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return !(_a == _b);
    }

    vec4& operator+=(vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        _a.x += _b.x;
        _a.y += _b.y;
        _a.z += _b.z;
        return _a;
    }


    vec4& operator*=(vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        _a.x *= _b.x;
        _a.y *= _b.y;
        _a.z *= _b.z;
        return _a;
    }

    vec4& operator*=(vec4& _vec, const f32 _b)
    {
        assert(false && "not ported yet");
        _vec.x *= _b;
        _vec.y *= _b;
        _vec.z *= _b;
        return _vec;
    }


    vec4& operator/=(vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        _a.x /= _b.x;
        _a.y /= _b.y;
        _a.z /= _b.z;
        return _a;
    }

    vec4& operator/=(vec4& _vec, const f32 _b)
    {
        assert(false && "not ported yet");
        _vec.x /= _b;
        _vec.y /= _b;
        _vec.z /= _b;
        return _vec;
    }

    vec4& operator-=(vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        _a.x -= _b.x;
        _a.y -= _b.y;
        _a.z -= _b.z;
        return _a;
    }


    f32 dot(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
    }


    f32 magnitude(const vec4& _v)
    {
        assert(false && "not ported yet");
        return sqrtf(dot(_v, _v));
    }


    f32 magnitude_sq(const vec4& _v)
    {
        assert(false && "not ported yet");
        return dot(_v, _v);
    }


    f32 distance(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return magnitude(_a - _b);
    }


    f32 distance_sq(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return magnitude_sq(_a - _b);
    }


    void normalize(vec4& _v)
    {
        assert(false && "not ported yet");
        _v = _v * (1.0f / magnitude(_v));
    }


    vec4 normalized(const vec4& _v)
    {
        assert(false && "not ported yet");
        return _v * (1.0f / magnitude(_v));
    }

    vec4 cross(const vec4& _a, const vec4& _b)
    {
        assert(false && "not ported yet");
        return vec4
        {
            _a.y * _b.z - _a.z * _b.y,
            _a.z * _b.x - _a.x * _b.z,
            _a.x * _b.y - _a.y * _b.x
        };
    }


    f32 angle(const vec4& l, const vec4& r)
    {
        assert(false && "not ported yet");
        return acosf(dot(l, r) / sqrtf(magnitude_sq(l) * magnitude_sq(r)));
    }


    vec4 project(const vec4& _length, const vec4& _direction)
    {
        assert(false && "not ported yet");
        return _direction * (dot(_length, _direction) / magnitude_sq(_direction));
    }



    vec4 perpendicular(const vec4& _length, const vec4& _direction)
    {
        assert(false && "not ported yet");
        return _length - project(_length, _direction);
    }


    vec4 reflection(const vec4& _vec, const vec4& _normal)
    {
        assert(false && "not ported yet");
        return _vec - _normal * (dot(_vec, _normal) *  2.0f);
    }

}


