#pragma once

#include "math/numbers/math_basics.h"
#include "vec3.h"
#include "vec4.h"
#include <cassert>


namespace rain::math
{
    struct mat3
    {
        vec3 data[3];
        vec3& operator[](u32 i) { return data[i]; }
        const vec3& operator[](u32 i) const { return data[i]; }
    };

    struct mat4
    {
        vec4 data[4];
        vec4& operator[](u32 i) { return data[i]; }
        const vec4& operator[](u32 i) const { return data[i]; }
    };

    inline void identity(mat3& mat)
    {
        mat[0] = vec3{ 1.0f, 0.0f, 0.0f };
        mat[1] = vec3{ 0.0f, 1.0f, 0.0f };
        mat[2] = vec3{ 0.0f, 0.0f, 1.0f };
    }

    inline void identity(mat4& mat)
    {
        mat[0] = vec4{ 1.0f, 0.0f, 0.0f, 0.0f };
        mat[1] = vec4{ 0.0f, 1.0f, 0.0f, 0.0f };
        mat[2] = vec4{ 0.0f, 0.0f, 1.0f, 0.0f };
        mat[4] = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
    }

    inline mat3 identity_mat3()
    {
        mat3 mat;
        mat[0] = vec3{ 1.0f, 0.0f, 0.0f };
        mat[1] = vec3{ 0.0f, 1.0f, 0.0f };
        mat[2] = vec3{ 0.0f, 0.0f, 1.0f };
        return mat;
    }

    inline mat4 identity_mat4()
    {
        mat4 mat;
        mat[0] = vec4{ 1.0f, 0.0f, 0.0f, 0.0f };
        mat[1] = vec4{ 0.0f, 1.0f, 0.0f, 0.0f };
        mat[2] = vec4{ 0.0f, 0.0f, 1.0f, 0.0f };
        mat[3] = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
        return mat;
    }



    inline vec4 operator*(const mat4& m, const vec4& v)
    {
        return vec4
        {

            m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
            m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3],
            m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3],
            m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3]
        };
    }

    inline vec4 operator*(const vec4& v, const mat4& m)
    {
        return vec4
        {
            m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3],
            m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3],
            m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3],
            m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3]
        };
    }

    inline mat4 operator*(const mat4& m1, const mat4& m2)
    {
        mat4 mat;
        mat[0] = m1[0] * m2[0][0] + m1[1] * m2[0][1] + m1[2] * m2[0][2] + m1[3] * m2[0][3];
        mat[1] = m1[0] * m2[1][0] + m1[1] * m2[1][1] + m1[2] * m2[1][2] + m1[3] * m2[1][3];
        mat[2] = m1[0] * m2[2][0] + m1[1] * m2[2][1] + m1[2] * m2[2][2] + m1[3] * m2[2][3];
        mat[3] = m1[0] * m2[3][0] + m1[1] * m2[3][1] + m1[2] * m2[3][2] + m1[3] * m2[3][3];
        return mat;
    }

    inline vec3 operator*(const mat4& m, const vec3& v)
    {
        return vec3
        {
            m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
            m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
            m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]
        };
    }

    inline vec3 operator*(const vec3& v, const mat4& m)
    {
        return vec3
        {
            m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
            m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
            m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]
        };
    }

    inline  mat4 mat4_cast(mat3& m3)
    {
        mat4 m4 = identity_mat4();
        m4[0] = vec4{ m3[0].x, m3[0].y, m3[0].z, 0.0f };
        m4[1] = vec4{ m3[1].x, m3[1].y, m3[1].z, 0.0f };
        m4[2] = vec4{ m3[2].x, m3[2].y, m3[2].z, 0.0f };
        m4[3] = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
        return m4;
    }

    inline mat4 translate(const mat4& m, const vec3& v)
    {
        mat4 mat(m);
        mat[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
        return mat;
    }

    inline mat4 scale(const mat4& mat, const vec3& scale)
    {
        mat4 scaled;
        scaled[0] = mat[0] * scale[0];
        scaled[1] = mat[1] * scale[1];
        scaled[2] = mat[2] * scale[2];
        scaled[3] = mat[3];
        return scaled;
    }

    inline mat4 scale(const mat4& mat, const vec4& scale)
    {
        mat4 scaled;
        scaled[0] = mat[0] * scale[0];
        scaled[1] = mat[1] * scale[1];
        scaled[2] = mat[2] * scale[2];
        scaled[3] = mat[3] * scale[3];
        return scaled;
    }
}