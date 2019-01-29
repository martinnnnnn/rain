#pragma once


#include "numbers/integers.h"
#include "numbers/float.h"
#include "numbers/math_basics.h"

#include "geometry/primitives/vec2.h"
#include "geometry/primitives/vec3.h"
#include "geometry/primitives/vec4.h"
#include "geometry/primitives/mat4.h"
#include "geometry/primitives/quaternion.h"
#include "geometry/primitives/plane.h"
#include "geometry/primitives/ray.h"
#include "geometry/primitives/quad.h"
#include "geometry/primitives/sphere.h"
#include "geometry/primitives/cube.h"
#include "geometry/primitives/transform.h"

#include "geometry/algorithms/quick_hull.h"

namespace rain::math
{
    inline f32 const*const value_ptr(const vec2& vec)
    {
        return &(vec.x);
    }

    inline f32 const*const value_ptr(const vec3& vec)
    {
        return &(vec.x);
    }

    inline f32 const*const value_ptr(const vec4& vec)
    {
        return &(vec.x);
    }

    inline f32 const*const value_ptr(const mat4& vec)
    {
        return &(vec[0].x);
    }

    inline f32 const*const value_ptr(const quat& vec)
    {
        return &(vec.x);
    }

    inline mat4 perspective(f32 fovy, f32 aspect, f32 z_near, f32 z_far)
    {
        const f32 tan_half_fovy = tan(fovy / 2.0f);

        mat4 m {};
        m[0][0] = 1.0f / (aspect * tan_half_fovy);
        m[1][1] = 1.0f / (tan_half_fovy);
        m[2][2] = z_far / (z_far - z_near);
        m[2][3] = 1.0f;
        m[3][2] = -(z_far * z_near) / (z_far - z_near);
        return m;
    }

    inline mat4 ortho(f32 left, f32 right, f32 bottom, f32 top)
    {
        mat4 m = identity_mat4();
        m[0][0] = 2.0f / (right - left);
        m[1][1] = 2.0f / (top - bottom);
        m[3][0] = -(right + left) / (right - left);
        m[3][1] = -(top + bottom) / (top - bottom);
        return m;
    }

    inline mat4 look_at(const vec3& eye, const vec3& center, const vec3& up)
    {
        const vec3 f(normalized(center - eye));
        const vec3 s(normalized(cross(up, f)));
        const vec3 u(cross(f, s));

        mat4 Result = identity_mat4();
        Result[0][0] = s.x;
        Result[1][0] = s.y;
        Result[2][0] = s.z;
        Result[0][1] = u.x;
        Result[1][1] = u.y;
        Result[2][1] = u.z;
        Result[0][2] = f.x;
        Result[1][2] = f.y;
        Result[2][2] = f.z;
        Result[3][0] = -dot(s, eye);
        Result[3][1] = -dot(u, eye);
        Result[3][2] = -dot(f, eye);
        return Result;
    }
}
