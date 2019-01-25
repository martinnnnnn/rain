#pragma once


#include "math/geometry/primitives/vec3.h"
#include "math/numbers/math_basics.h"

namespace rain::math
{

    struct mesh
    {
        vec3* vertices;
        vec3* normals;
        u32* vertices_indices;
        u32* normals_indices;
        u32 vertices_count;
        u32 vertices_indices_count;
        u32 normals_count;
        u32 normal_indices_count;
    };

    void quick_hull(vec3 const *  vertices, const u32 indices, mesh* output);
}