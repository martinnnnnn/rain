#pragma once


#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"

namespace rain::math
{

    struct transform
    {
        vec3 position;
        vec3 lastPosition;

        quat orientation;
        quat lastOrientation;

        vec3 scale;
        vec3 lastScale;
    };

    inline mat4 get_transform_matrix(const transform& t)
    {
        return translate(identity_mat4(), t.position) * mat4_cast(t.orientation) * scale(identity_mat4(), t.scale);
    }

}