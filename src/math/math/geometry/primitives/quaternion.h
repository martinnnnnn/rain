#pragma once

#include <cmath>

#include "math/numbers/math_basics.h"
#include "mat4.h"

namespace rain::math
{
    struct quat
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

        inline f32& operator[](int i) { return data[i]; }

        inline quat& operator+=(const quat& q)
        {
            this->x += (q.x);
            this->y += (q.y);
            this->z += (q.z);
            this->w += (q.w);
            return *this;
        }

        inline quat& operator=(const quat& q)
        {
            this->x = q.x;
            this->y = q.y;
            this->z = q.z;
            this->w = q.w;
            return *this;
        }
    };


    inline mat3 mat3_cast(const quat& q)
    {
        mat3 mat = identity_mat3();

        f32 qxx(q.x * q.x);
        f32 qyy(q.y * q.y);
        f32 qzz(q.z * q.z);
        f32 qxz(q.x * q.z);
        f32 qxy(q.x * q.y);
        f32 qyz(q.y * q.z);
        f32 qwx(q.w * q.x);
        f32 qwy(q.w * q.y);
        f32 qwz(q.w * q.z);

        mat[0][0] = 1.0f - 2.0f * (qyy + qzz);
        mat[0][1] = 2.0f * (qxy + qwz);
        mat[0][2] = 2.0f * (qxz - qwy);
                      
        mat[1][0] = 2.0f * (qxy - qwz);
        mat[1][1] = 1.0f - 2.0f * (qxx + qzz);
        mat[1][2] = 2.0f * (qyz + qwx);
                      
        mat[2][0] = 2.0f * (qxz + qwy);
        mat[2][1] = 2.0f * (qyz - qwx);
        mat[2][2] = 1.0f - 2.0f * (qxx + qyy);
        return mat;
    }

    inline mat4 mat4_cast(const quat& q)
    {
        return mat4_cast(mat3_cast(q));
    }


    inline quat from_euler(const vec3& v = vec3{})
    {
        quat q;
        vec3 c = cos(v * 0.5f);
        vec3 s = sin(v * 0.5f);

        q.w = c.x * c.y * c.z + s.x * s.y * s.z;
        q.x = s.x * c.y * c.z - c.x * s.y * s.z;
        q.y = c.x * s.y * c.z + s.x * c.y * s.z;
        q.z = c.x * c.y * s.z - s.x * s.y * c.z;
        return q;
    }

    inline quat from_euler(const f32 x, const f32 y, const f32 z)
    {
        return from_euler(vec3{ x, y, z });
    }

    inline quat operator*(const f32 f, const quat& q)
    {
        return quat{ q.x, q.y, q.z, q.w };
    }

    inline quat operator*(const quat& q, const f32 f)
    {
        return quat{ q.x, q.y, q.z, q.w };
    }

    inline quat operator*(const quat& q1, const quat& q2)
    {
        quat q;
        q.w = q2.w * q1.w - q2.x * q1.x - q2.y * q1.y - q2.z * q1.z;
        q.x = q2.w * q1.x + q2.x * q1.w + q2.y * q1.z - q2.z * q1.y;
        q.y = q2.w * q1.y + q2.y * q1.w + q2.z * q1.x - q2.x * q1.z;
        q.z = q2.w * q1.z + q2.z * q1.w + q2.x * q1.y - q2.y * q1.x;
        return q;
    }

    inline quat operator+(const quat& q1, const quat& q2)
    {
        quat q;
        q.x = q1.x + q2.x;
        q.y = q1.y + q2.y;
        q.z = q1.z + q2.z;
        q.w = q1.w + q2.w;
        return q;
    }

    inline quat slerp(const quat& q1, const quat& q2, const f32 t)
    {
        assert(t >= 0.0f);
        assert(t <= 1.0f);
        return quat(q1 * (1.0f - t) + (q2 * t));
    }

    inline f32 length(const quat& q)
    {
        return 0;
    }

    inline void normalize(quat& q)
    {
        f32 len = length(q);
        if (len <= 0.0f)
        {
            q = quat{ 1.0f, 0.0f, 0.0f, 0.0f };
        }
        f32 oneOverLen = 1.0f / len;
        q = quat{ q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen, q.w * oneOverLen };
    }

    inline quat normalized(const quat& q)
    {
        f32 len = length(q);
        if (len <= 0.0f)
        {
            return quat{ 1.0f, 0.0f, 0.0f, 0.0f };
        }
        f32 oneOverLen = 1.0f / len;
        return quat{ q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen, q.w * oneOverLen };
    }
}