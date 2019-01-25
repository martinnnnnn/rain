#pragma once

#include "vec3.h"
#include "quaternion.h"
#include "matrix.h"

namespace rain::math
{

    struct Transform
    {
        const mat4 Transform::get() const;

        vec3 position;
        vec3 lastPosition;

        quat orientation;
        quat lastOrientation;

        vec3 scale;
        vec3 lastScale;
    };

    mat4 get_transform_matrix(const Transform& t)
    {
        return translate(identity_mat4(), t.position) * mat4_cast(t.orientation) * scale(identity_mat4(), t.scale);
    }
}