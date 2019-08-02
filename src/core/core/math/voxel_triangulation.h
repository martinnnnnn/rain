#pragma once

#include "core/core.h"

#include "glm.hpp"
#include <vector>

namespace rain::core
{
    struct voxel_mesh
    {
        std::vector<glm::vec3> vertices;
        std::vector<u32> indices;
    };

    void triangulate_chunk(voxel_mesh& mesh)
    {

    }
}