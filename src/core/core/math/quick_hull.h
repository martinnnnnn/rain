#pragma once


#include "core/core.h"
#include "glm.hpp"

namespace rain::core
{

    struct mesh
    {
        glm::vec3* vertices;
        glm::vec3* normals;
        u32* vertices_indices;
        u32* normals_indices;
        u32 vertices_count;
        u32 vertices_indices_count;
        u32 normals_count;
        u32 normal_indices_count;
    };

    void quick_hull(glm::vec3 const *  vertices, const u32 indices, mesh* output);
}