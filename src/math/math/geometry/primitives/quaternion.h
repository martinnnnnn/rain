#pragma once

#include "math/numbers/math_basics.h"
#include "matrix.h"

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

        f32& operator[](int i) { return data[i]; }
    };


    mat3 mat3_cast(const quat& q)
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

    mat4 mat4_cast(const quat& q)
    {
        return mat4_cast(mat3_cast(q));
    }
}